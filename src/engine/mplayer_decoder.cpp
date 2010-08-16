#include <glibmm/fileutils.h>
#include <glibmm/regex.h>
#include <glibmm/stringutils.h>
#include <iostream>
#include <stdexcept>

#include "settings/setting.h"

#include "engine/mplayer_decoder.h"

using namespace Glib;
using namespace std;

MplayerDecoder::MplayerDecoder () :
pid (0) {
}

MplayerDecoder::~MplayerDecoder () {
    close ();
}

bool MplayerDecoder::open (const Glib::ustring& filename) {
    Setting& set = Settings::get_app_setting ();
    vector<ustring> cmd;
    cmd.push_back ("/usr/bin/mplayer");
    cmd.push_back ("-really-quiet");
    cmd.push_back ("-novideo");
    cmd.push_back ("-noconsolecontrols");
    cmd.push_back ("-ao");
    cmd.push_back ("pcm:file=/dev/stdout");
    cmd = set.read_string_list ("audio", "decoder_cmd", cmd);
    cmd.push_back (filename);

    try {
        spawn_async_with_pipes ("", cmd, SPAWN_STDERR_TO_DEV_NULL, sigc::slot<void>(),
                &pid, NULL, &source_fd, NULL);

        try {
            source = IOChannel::create_from_fd (source_fd);
            source->set_encoding ("");

            unsigned char header[44];
            size_t read = 0;
            size_t remain = 44;
            for (char *target = (char*) header; remain > 0; target += read, remain -= read) {
                if (source->read (target, remain, read) != IO_STATUS_NORMAL)
                    return false;
            }

            if (header[0] != 'R' || header[1] != 'I' || header[2] != 'F' || header[3] != 'F') {
                return false;
            }

            if (header[8] != 'W' || header[9] != 'A' || header[10] != 'V' || header[11] != 'E') {
                return false;
            }

            if (header[12] != 'f' || header[13] != 'm' || header[14] != 't' || header[15] != ' ') {
                return false;
            }

            if (header[20] != 1 || header[21] != 0) {
                return false;
            }

            size_t channels = header[22] + header[23] * 256;
            size_t sample = header[34] + header[35] * 256;

            if (sample != 8 && sample != 16) {
                return false;
            }

            switch (channels) {
            case 1:
                format = (sample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
                break;
            case 2:
                format = (sample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
                break;
            default:
                return false;
            }

            frequency = ((header[27] * 256 + header[26]) * 256 + header[25]) * 256 + header[24];

            return true;
        } catch (IOChannelError) {
        } catch (FileError) {
            spawn_close_pid (pid);
            pid = 0;
        }
    } catch (SpawnError) {
    }
    return false;
}

void MplayerDecoder::close () {
    if (pid != 0) {
        source->close (false);
        spawn_close_pid (pid);
        pid = 0;
    }
}

ALenum MplayerDecoder::get_format () const {
    return format;
}

ALsizei MplayerDecoder::get_frequency () const {
    return frequency;
}

size_t MplayerDecoder::read (char* buffer, size_t len) {
    size_t remain = len;
    size_t read = 0;
    try {

        for (char* pos = buffer; remain > 0; pos += read, remain -= read) {
            switch (source->read (pos, remain, read)) {
            case IO_STATUS_NORMAL:
                break;
            default:
                return len - remain;
            }
        }
        return len;

    } catch (IOChannelError) {
        return len - remain;
    }
}

double MplayerDecoder::get_length (const Glib::ustring& filename) {
    Setting& set = Settings::get_app_setting ();
    vector<ustring> cmd;
    cmd.push_back ("/usr/bin/mplayer");
    cmd.push_back ("-identify");
    cmd.push_back ("-novideo");
    cmd.push_back ("-ao");
    cmd.push_back ("null");
    cmd.push_back ("-frames");
    cmd.push_back ("0");
    cmd = set.read_string_list ("audio", "info_cmd", cmd);
    cmd.push_back (filename);

    ustring length_pattern = set.read_string ("audio", "length_pattern", "ID_LENGTH=([0-9]+\\.[0-9]+)");
    RefPtr<Regex> pat = Regex::create (length_pattern);

    Pid info_pid;
    int info_fd;
    ustring line;
    double result = -1;

    try {
        spawn_async_with_pipes ("", cmd, SPAWN_STDERR_TO_DEV_NULL, sigc::slot<void>(),
                &info_pid, NULL, &info_fd, NULL);

        try {
            RefPtr<IOChannel> info = IOChannel::create_from_fd (info_fd);

            while (info->read_line (line) == IO_STATUS_NORMAL) {
                vector<ustring> groups = pat->split (line);
                if (groups.size () > 1) {
                    try {
                        result = Ascii::strtod (groups[1]);
                        break;
                    } catch (overflow_error) {
                    } catch (underflow_error) {
                    }
                }
            }


        } catch (IOChannelError) {
        } catch (FileError) {
        }

        spawn_close_pid (info_pid);

    } catch (SpawnError) {
    }
    return result;
}


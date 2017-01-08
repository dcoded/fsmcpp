#include <fsm/state_machine.hpp>
#include <fsm/cond/if_all.hpp>
#include <fsm/cond/if_any.hpp>
#include <fsm/cond/if_not.hpp>

enum class cd_tray_state
{
    CD_TRAY_OPEN,
    CD_TRAY_CLOSED,

    CD_TRAY_OPENING,
    CD_TRAY_CLOSING
};

bool disk_playing = true;

bool is_playing()
{
    return disk_playing;
}

bool is_not_playing()
{
    return !disk_playing;
}

void stop_disk()
{
    std::cout << "stopping disk...\n";
    disk_playing = false;
}

void start_disk()
{
    std::cout << "playing disk...\n";
    disk_playing = true;
}


int main(int argc, char* argv[])
{
    fsm::state_machine<cd_tray_state> fsm {
        cd_tray_state::CD_TRAY_CLOSED, // initial state
        {
            {
                cd_tray_state::CD_TRAY_CLOSED,
                cd_tray_state::CD_TRAY_OPENING,
                [] () {},
                if_not(is_playing)
            },

            {
                cd_tray_state::CD_TRAY_CLOSED,
                cd_tray_state::CD_TRAY_CLOSED,
                stop_disk,
                is_playing
            },

            {
                cd_tray_state::CD_TRAY_OPENING,
                cd_tray_state::CD_TRAY_OPEN
            },

            {
                cd_tray_state::CD_TRAY_OPEN,
                cd_tray_state::CD_TRAY_CLOSING
            },

            {
                cd_tray_state::CD_TRAY_CLOSING,
                cd_tray_state::CD_TRAY_CLOSING,
                start_disk,
                if_not(is_playing)
            },

            {
                cd_tray_state::CD_TRAY_CLOSING,
                cd_tray_state::CD_TRAY_CLOSED,
                [] {},
                if_all(is_playing)
            },
        }
    };


    for (int i = 0; i < 12; i++)
    {
        fsm.next();
    }

    return 0;
}

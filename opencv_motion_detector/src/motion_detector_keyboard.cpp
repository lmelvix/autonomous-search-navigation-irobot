#include "ros/ros.h"
#include "motion_detector/Mode.h"
#include "std_srvs/Empty.h"
#include "std_msgs/Bool.h"
#include "std_srvs/Empty.h"

int main(int argc, char **argv)
{
        ros::init(argc, argv, "motion_detector_keyboard");
        ros::NodeHandle node;


        // Setup client to exit session.
        ros::ServiceClient exitClient = node.serviceClient<std_srvs::Empty>("exit");

        /***************************
         *
         * SETUP MODE CLIENT HERE.
         *
         ***************************/

        ros::ServiceClient modeClient =
          node.serviceClient<motion_detector::Mode>("mode");



        std::string mode = "MOG2";

        while(ros::ok())
        {
                std::string input;

                // Clear screen.
                std::cout << "\033[2J\033[1;1H";

                // Print menu.
                std::cout << "MAIN MENU" << std::endl;
                std::cout << "---------------------------------" << std::endl;
                std::cout << "Current mode: " << mode << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "  M - Enter mode" << std::endl;
                std::cout << "  E - Exit" << std::endl;
                std::cout << "---------------------------------" << std::endl;
                std::cout << "Choice: ";
                std::cin >> input;
                std::cout << std::endl;

                if (input == "M" || input == "m")
                {
                        // Handle the enter mode choice.
                        // Clear screen.
                        std::cout << "\033[2J\033[1;1H";
                        // Print mode menu.
                        std::cout << "MAIN MENU" << std::endl;
                        std::cout << "---------------------------------" << std::endl;
                        std::cout << "Current mode: " << mode << std::endl;
                        std::cout << "Options:" << std::endl;
                        std::cout << "  R - RAW" << std::endl;
                        std::cout << "  F - FARNEBACK" << std::endl;
                        std::cout << "  M - MOG2" << std::endl;
                        std::cout << "  E - Exit" << std::endl;
                        std::cout << "---------------------------------" << std::endl;
                        std::cout << "Enter mode: ";
                        std::cin >> input;
                        std::cout << std::endl;

                        // Check if the mode is valid.
                        if (input == "R" || input == "r")
                        {
                                mode = "RAW";

                        }
                        else if (input == "F" || input == "f")
                        {
                                mode = "FARNEBACK";
                        }
                        else if (input == "M" || input == "m")
                        {
                                mode = "MOG2";
                        }
                        else if (input == "E" || input == "e")
                        {
                                std::cout << "Exiting..." << std::endl;

                                // Call exit service.
                                std_srvs::Empty exitSrv;
                                exitClient.call(exitSrv);

                                return 0;
                        }
                        else
                        {
                                std::cout << "'" << input << "' is not a valid mode." << std:: endl;
                                std::cout << "---------------------------------" << std::endl;

                                // Continue message.
                                std::cout << "Press any key to continue.";
                                getchar();
                                getchar();
                                continue;
                        }

                        /********************************
                         *
                         * HANDLE SETTING THE MODE HERE.
                         *
                         ********************************/

                        // Setup the service request.
                        motion_detector::Mode srvMode;
                        srvMode.request.mode = mode;
                        std_msgs::Bool a;

                        if (modeClient.call(srvMode))
                        {
                                a.data = srvMode.response.success;
                                std::cout << "Success: " << a << std::endl;
                        }
                        else
                        {
                                //std::cout << "Success: " << srvMode.response.success << std::endl;
                                std::cout << "Failed to call service";

                                return 1;
                        }






                        std::cout << "Mode set to " << mode << std::endl;
                        std::cout << "---------------------------------" << std::endl;

                        // Continue message.
                        std::cout << "Press any key to continue.";
                        getchar();
                        getchar();
                        continue;
                }
                else if (input == "E" || input == "e")
                {
                        std::cout << "Exiting..." << std::endl;

                        // Call exit service.
                        std_srvs::Empty exitSrv;
                        exitClient.call(exitSrv);

                        return 0;
                }
                else
                {
                        std::cout << "'" << input << "' is not a valid choice." << std:: endl;
                        std::cout << "---------------------------------" << std::endl;

                        // Continue message.
                        std::cout << "Press any key to continue.";
                        getchar();
                        getchar();
                        continue;
                }
        }

        return 0;
}



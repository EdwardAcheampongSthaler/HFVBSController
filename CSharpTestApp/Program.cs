using System;

using STH.BiometricDevicesController;
using STH.BiometricDevicesController.Interfaces;

namespace CSharpTestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            var wrapper = new BiometricDeviceController();

            // enter a unsafe block
            unsafe
            {
                // initialise
                wrapper.Initialise(new HFVBSBiometricDeviceConfigurationData());
                Console.WriteLine("Please type one of the following commands to perform the action:");
                Console.WriteLine("ENROLL");
                Console.WriteLine("IDENTIFY");
                Console.WriteLine("VERIFY");
                Console.WriteLine("Press Enter To run the command once finished typing");

                var command = Console.ReadLine();
                if (!string.IsNullOrEmpty(command))
                {
                    switch (command.ToUpper())
                    {
                        case "ENROLL":
                            wrapper.Enroll();
                            break;
                        case "IDENTIFY":
                            wrapper.Identify();
                            break;
                        case "VERIFY":
                            wrapper.Verify();
                            break;
                        default:
                            Console.WriteLine("ERROR: INVALID COMMAND {UNKNOWN}:" + command);

                            wrapper.CancelOperations();
                            break;
                    }                
                }
            }

            Console.Read();
        }
    }
}

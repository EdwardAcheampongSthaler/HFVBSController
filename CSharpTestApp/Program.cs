using System;

using STH.BiometricDevicesController;
using STH.BiometricDevicesController.Interfaces;

namespace CSharpTestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            while (Console.ReadLine() != "end") { 
            var wrapper = new BiometricDeviceController();
            var output = new InteractionResult();
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
                            output = wrapper.Enroll();
                            break;
                        case "IDENTIFY":
                            output = wrapper.Identify();
                            break;
                        case "VERIFY":
                            output = wrapper.Verify();
                            break;
                        default:
                            Console.WriteLine("ERROR: INVALID COMMAND {UNKNOWN}:" + command);

                            output = wrapper.CancelOperations();
                            break;
                    }
                }
            }

            Console.Write(output.Data.ToString());
            //Console.Read();
        }
    }
    }
}

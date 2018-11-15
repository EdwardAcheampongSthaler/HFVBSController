using STH.BiometricDeviceController.Interfaces;

namespace STH.BiometricDeviceController
{
    public class HfvbsController:IBiometricDeviceController
    {
        public InteractionResult Initialise(IComparableConfigurationData configuration)
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult Enroll()
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult Verify()
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult ControlBiometricDevice(IControlDeviceData controlCommandData)
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult Identify()
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult CancelOperations()
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult Terminate()
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult ResetBiometricDevice()
        {
            throw new System.NotImplementedException();
        }
    }

    
}

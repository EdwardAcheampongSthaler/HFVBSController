using STH.BiometricDevicesController.Interfaces;

namespace STH.BiometricDevicesController
{
    public class BiometricDeviceController : IBiometricDeviceController
    {
        public InteractionResult Initialise(IBiometricDeviceConfigurationData configuration)
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

        public InteractionResult ControlBiometricDevice(IBiometricDeviceControlData controlCommandData)
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
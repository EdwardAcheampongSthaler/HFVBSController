using STH.BiometricDevicesController.Interfaces;
using HFVBSControllerWrapper;

namespace STH.BiometricDevicesController
{
    public class BiometricDeviceController : IBiometricDeviceController
    {
        private HFVBSControlerWrapperClass _hfvbsControlerWrapperClass;
        // create singleton access.
        public unsafe HFVBSControlerWrapperClass HfvbsControlerWrapperClass
        {
            get => _hfvbsControlerWrapperClass ?? (_hfvbsControlerWrapperClass = new HFVBSControlerWrapperClass());
            set => _hfvbsControlerWrapperClass = value;
        }

        public BiometricDeviceController()
        {
            // new an instance of our dll.
            unsafe
            {
                HfvbsControlerWrapperClass = new HFVBSControlerWrapperClass();
            }
        }

        public InteractionResult Initialise(IBiometricDeviceConfigurationData configuration)
        {
            unsafe
            {
                // pass into the wrapped classes the parameters 
                var result = HfvbsControlerWrapperClass.Initialise();
                // create a new Interaction Result object from response
                return new InteractionResult();

            }
        }

        public InteractionResult Enroll()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Enroll();
                if (response == null) return null;
                // create a new Interaction Result object from response
                return new InteractionResult()
                {
                    Data = new byte[100]
                };
            }
        }

        public InteractionResult Verify()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Verify();
                if (response == null) return null;
                // create a new Interaction Result object from response
                return new InteractionResult();
            }
        }

        public InteractionResult ControlBiometricDevice(IBiometricDeviceControlData controlCommandData)
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.ControlBiometricDevice();
                if (response == null) return null;
                // create a new Interaction Result object from response
                return new InteractionResult();
            }
        }

        public InteractionResult Identify()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Identify();
                if (response == null) return null;
                // create a new Interaction Result object from response
                return new InteractionResult();
            }
        }

        public InteractionResult CancelOperations()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.CancelOperations();
                if (response == null) return null;
                // create a new Interaction Result object from response
                return new InteractionResult();
                //return new InteractionResult()
                //{
                //    Message = response.Message,
                //    StatusCode = response.StatusCode,
                //    Success = response.Success

                //};
            }
        }

        public InteractionResult Terminate()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Terminate();
                if (response == null) return null;
                // create a new Interaction Result object from response
                return new InteractionResult();
            }
        }

        public InteractionResult Shutdown()
        {
            throw new System.NotImplementedException();
        }

        public InteractionResult HealthCheck()
        {
            throw new System.NotImplementedException();
        }
    }
}
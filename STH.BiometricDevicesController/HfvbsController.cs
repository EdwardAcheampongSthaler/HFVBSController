using System.Net;
using System.Runtime.Remoting.Messaging;
using HFVBSControllerWrapper;
using STH.BiometricDevicesController.Interfaces;

namespace STH.BiometricDevicesController
{
    public class HfvbsController:IBiometricDeviceController
    {
        private HFVBSControlerWrapperClass _hfvbsControlerWrapperClass;
        // create singleton access.
       public unsafe HFVBSControlerWrapperClass HfvbsControlerWrapperClass
        {
            get => _hfvbsControlerWrapperClass ?? (_hfvbsControlerWrapperClass = new HFVBSControlerWrapperClass());
            set => _hfvbsControlerWrapperClass = value;
        }

        public HfvbsController()
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
                HfvbsControlerWrapperClass.Initialise();
               
                return new InteractionResult();

            }
        }

        public InteractionResult Enroll()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Enroll();
                if (response == null) return null;
                return new InteractionResult();
            }
        }

        public InteractionResult Verify()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Verify();
                if (response == null) return null;
                return new InteractionResult();
            }
        }

        public InteractionResult ControlBiometricDevice(IBiometricDeviceControlData controlCommandData)
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.ControlBiometricDevice();
                if (response == null) return null;
                return new InteractionResult();
            }
        }

        public InteractionResult Identify()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.Identify();
                if (response == null) return null;
                return new InteractionResult();
            }
        }

        public InteractionResult CancelOperations()
        {
            unsafe
            {
                var response = HfvbsControlerWrapperClass.CancelOperations();
                if (response == null) return null;
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
                return new InteractionResult();
            }
        }

    }

    
}

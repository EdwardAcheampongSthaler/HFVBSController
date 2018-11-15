using STH.BiometricDevicesController.Interfaces;

namespace STH.BiometricDevicesController
{
    public class HFVBSBiometricDeviceConfigurationData : IBiometricDeviceConfigurationData
    {
        public string BspDefaultUuid => "e41a9357-67df-44d5-80e3-64935d21b642";
        public int BioApiVersion => 0x20;
    }
}
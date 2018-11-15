namespace STH.BiometricDevicesController.Interfaces
{
    //BSP_DEFAULT_UUID	( "e41a9357-67df-44d5-80e3-64935d21b642" )
    //m_BioAPIVersion 0x20
    public interface IBiometricDeviceConfigurationData
    {
        string BspDefaultUuid { get;  }
        int BioApiVersion { get; }
    }
}
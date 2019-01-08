namespace STH.BiometricDevicesController.Interfaces
{
    /// <summary>
    /// Biometric Device Controller designed to provide set of
    /// generic commands for biometric devices to be implemented
    /// </summary>
    public interface IBiometricDeviceController
    {
        InteractionResult Initialise(IBiometricDeviceConfigurationData configuration);
        InteractionResult Enroll();
        InteractionResult EnrollAndVerify();
        InteractionResult Verify();
        InteractionResult ControlBiometricDevice(IBiometricDeviceControlData controlCommandData);
        InteractionResult Identify();
        InteractionResult CancelOperations();
        InteractionResult Terminate();
        InteractionResult Shutdown();
        InteractionResult HealthCheck();
    }
}
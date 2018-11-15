namespace STH.BiometricDeviceController.Interfaces
{
    /// <summary>
    /// Biometric Device Controller designed to provide set of
    /// generic commands for biometric devices to be implemented
    /// </summary>
    public interface IBiometricDeviceController
    {
        InteractionResult Initialise(IComparableConfigurationData configuration);
        InteractionResult Enroll();
        InteractionResult Verify();
        InteractionResult ControlBiometricDevice(IControlDeviceData controlCommandData);
        InteractionResult Identify();
        InteractionResult CancelOperations();
        InteractionResult Terminate();
        InteractionResult ResetBiometricDevice();
    }
}
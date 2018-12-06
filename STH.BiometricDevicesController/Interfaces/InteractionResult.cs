namespace STH.BiometricDevicesController.Interfaces
{
    public class InteractionResult
    {
        public string Message { get; set; }
        public int StatusCode { get; set; }
        public bool Success { get; set; }
        public string Data { get; set; }
    }
}
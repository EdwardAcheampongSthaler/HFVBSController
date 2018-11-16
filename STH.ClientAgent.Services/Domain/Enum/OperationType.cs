namespace STH.BiometricIdentity.Domain.Enum
{
    public enum OperationType
    {
        Enrol = 0,
        ReEnrol = 1,
        Identify = 2,
        Verify = 3,
        Remove = 4,
        GetUsers = 5,
        GetAccesses = 6,
        GetMetrics = 7,
        DeleteAllUsers = 8,
        DeleteAllTransaction = 9
    }
}
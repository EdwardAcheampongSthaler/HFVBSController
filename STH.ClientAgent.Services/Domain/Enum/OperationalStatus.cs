namespace STH.BiometricIdentity.Domain.Enum
{
    public enum OperationalStatus
    {
        Succeed = 0,
        Failed = 1,
        UserNotFound = 2,
        NotMatched = 3,
        Duplicate = 4,
        MultipleMatched = 5,
        InvalidUUID = 6,
        InvalidEnrolTemplate = 7,
        InvalidVerifyTemplate = 8,
        DuplicateUUID = 9,
        UnauthorisedClientRequest = 10,
        InternalFVAEError = 11,
        TimebombRecoveryInProgress = 12,
        EnrollDuplicateHasBeenRemoved = 13,
        LicenceExpired = 14,
        ServerStartUpError = 15,
        NoEnrolledUsers = 16,
        TooManyTemplates = 17
    }
}
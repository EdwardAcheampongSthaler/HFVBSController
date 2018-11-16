namespace STH.BiometricIdentity.Domain.Enum
{
    public enum AuthenticationResultCode
    {
        Succeed,
        Failed,
        UserNotFound,
        NotMatched,
        Duplicate,
        MultipleMatched,
        InvalidUUID,
        InvalidEnrolTemplate,
        InvalidVerifyTemplate,
        DuplicateUUID,
        UnauthorisedClientRequest,
        InternalFVAEError,
        TimebombRecoveryInProgress,
        EnrollDuplicateHasBeenRemoved,
        LicenceExpired,
        ServerStartUpError,
        NoEnrolledUsers,
        TooManyTemplates,
    }
}
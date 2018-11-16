using STH.BiometricIdentity.Domain;

namespace STH.BiometricIdentity.Interface
{
    public interface IVerifyBiometricIdentityRecord
    {
        BirVerificationResult VerifyBiometricIdentityRecord(BirTemplateModel model);
    }
}
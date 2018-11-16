using STH.BiometricIdentity.Domain;

namespace STH.BiometricIdentity.Interface
{
    public interface IIdentifyBiometricIdentityRecord
    {
        BirIdentificationResult IdentifyBiometricIdentityRecord(BirTemplateModel model);
    }
}
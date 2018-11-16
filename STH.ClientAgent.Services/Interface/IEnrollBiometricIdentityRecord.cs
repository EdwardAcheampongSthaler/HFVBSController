using STH.BiometricIdentity.Domain;

namespace STH.BiometricIdentity.Interface
{
    public interface IEnrollBiometricIdentityRecord
    {
        BirEnrollmentResult EnrollBiometricIdentityRecord(BirTemplateModel model);
    }
}
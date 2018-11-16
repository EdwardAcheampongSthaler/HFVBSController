using System;
using STH.BiometricIdentity.Interface;

namespace STH.BiometricIdentity.Domain
{
    public class BiometricIdentityRecordService :
        IEnrollBiometricIdentityRecord,
        IReEnrollBiometricIdentityRecord,
        IVerifyBiometricIdentityRecord,
        IIdentifyBiometricIdentityRecord,
        IRemoveBiometricIdentityRecord
    {
        public BiometricIdentityRecordService()
        {

        }

        public BirEnrollmentResult EnrollBiometricIdentityRecord(BirTemplateModel model)
        {
            throw new NotImplementedException();
        }

        public BirReEnrollmentResult ReEnrollBiometricIdentityRecord(BirTemplateModel model, Guid uuid)
        {
            throw new NotImplementedException();
        }

        public BirVerificationResult VerifyBiometricIdentityRecord(BirTemplateModel model)
        {
            throw new NotImplementedException();
        }

        public BirIdentificationResult IdentifyBiometricIdentityRecord(BirTemplateModel model)
        {
            throw new NotImplementedException();
        }

        public BirRemovalResult RemoveBiometricIdentityRecord(Guid uuid)
        {
            throw new NotImplementedException();
        }
    }
}
using System;
using STH.BiometricIdentity.Domain;

namespace STH.BiometricIdentity.Interface
{
    public interface IRemoveBiometricIdentityRecord
    {
        BirRemovalResult RemoveBiometricIdentityRecord(Guid uuid);
    }
}
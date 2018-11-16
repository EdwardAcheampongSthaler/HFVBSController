using System;
using STH.BiometricIdentity.Domain.Enum;

namespace STH.BiometricIdentity.Domain
{
    public class BirTemplateModel
    {
        public Guid UUID { get; set; }
        public string Template { get; set; }
        public DateTime CaptureDateTime { get; set; }
        public Hand Hand { get; set; }
        public Finger Finger { get; set; }
        public TemplateVersion Version { get; set; }

    }
}
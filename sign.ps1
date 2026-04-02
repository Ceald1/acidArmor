# Run as Administrator

$certName = "AcidArmorTestCert"
$driverPath = "C:\Users\Administrator\Desktop\*.sys"  # <-- change this

# Create self-signed cert
$cert = New-SelfSignedCertificate `
    -Subject "CN=$certName" `
    -CertStoreLocation "Cert:\LocalMachine\My" `
    -KeyUsage DigitalSignature `
    -Type CodeSigningCert `
    -HashAlgorithm SHA256

# Trust it (both root and code signing stores)
$store = [System.Security.Cryptography.X509Certificates.X509Store]::new("Root", "LocalMachine")
$store.Open("ReadWrite")
$store.Add($cert)
$store.Close()

$store2 = [System.Security.Cryptography.X509Certificates.X509Store]::new("TrustedPublisher", "LocalMachine")
$store2.Open("ReadWrite")
$store2.Add($cert)
$store2.Close()

# Sign the driver
$thumb = $cert.Thumbprint
Get-ChildItem "C:\Users\Administrator\Desktop\*.sys" | ForEach-Object {
    Set-AuthenticodeSignature -FilePath $driverPath `
        -Certificate (Get-Item "Cert:\LocalMachine\My\$thumb") `
        -HashAlgorithm SHA256
}
Write-Host "Done. Thumbprint: $thumb"
Write-Host "Verify with: Get-AuthenticodeSignature '$driverPath'"

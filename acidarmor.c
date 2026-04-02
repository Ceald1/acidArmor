#include <ntddk.h>
#include <wdm.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject,
                     PUNICODE_STRING RegistryPath) {
  UNREFERENCED_PARAMETER(RegistryPath);
  DbgPrint("KmdfHelloWorld: DriverEntry\n");
  // Example: just return success
  return STATUS_SUCCESS;
}

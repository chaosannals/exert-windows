#include <ntddk.h>

CONST CHAR DEVICE_NAME[] = "DemoDeviceName";

PDEVICE_OBJECT device = NULL;

VOID DemoDriverUnload(PDRIVER_OBJECT driver) {
	if (device) {
		IoDeleteDevice(device);
	}
	UNREFERENCED_PARAMETER(driver);
	KdPrint("wdm1d unload.");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING registryPath) {
	driver->DriverUnload = DemoDriverUnload;
	UNICODE_STRING deviceName = RTL_CONSTANT_STRING(DEVICE_NAME);
	NTSTATUS ds = IoCreateDevice(driver, 0, &deviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, &device);
	if (!NT_SUCCESS(ds)) {
		KdPrint("wdm1d failed create device on entry.");
		return STATUS_FAILED_DRIVER_ENTRY;
	}
	UNREFERENCED_PARAMETER(registryPath);
	KdPrint("wdm1d entry. %wZ", registryPath);
	return STATUS_SUCCESS;
}
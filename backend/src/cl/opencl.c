#include <stdio.h>
#include <stddef.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/opencl.h>
#include <CL/cl.h>

int main() {
    cl_uint platformCount;
    clGetPlatformIDs(0, NULL, &platformCount);
    if (platformCount == 0)
    {
        printf("Found 0 platforms!\n");
        return EXIT_FAILURE;
    }
    cl_platform_id platform[platformCount];
    clGetPlatformIDs(platformCount, platform, NULL);
    for (cl_uint i = 0; i < platformCount; i++) {
        cl_char string [10240] = {0};
        clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, sizeof(string), string, NULL);
        printf("Platform %u: %s\n", i, string);
        clGetPlatformInfo(platform[i], CL_PLATFORM_VERSION, sizeof(string), string, NULL);
        printf("Version %u: %s\n", i, string);
        clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, sizeof(string), string, NULL);
        printf("Vendor %u: %s\n", i, string);
        cl_uint deviceCount;
        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        cl_device_id device[deviceCount];
        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, deviceCount, device, NULL);
        for (cl_uint j = 0; j < deviceCount; j++) {
            cl_char deviceName[10240] = {0};
            clGetDeviceInfo(device[j], CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
            printf("  Device %u: %s\n", j, deviceName);
            clGetDeviceInfo(device[j], CL_DEVICE_OPENCL_C_VERSION, sizeof(string), &string, NULL);
            printf("  OpenCL C Version: %s\n", string);
            cl_uint num;
            cl_uint max_work_item_dimensions;
            clGetDeviceInfo(device[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num, NULL);
            printf("  Max Compute Units: %u\n", num);

            cl_ulong mem_size;
            clGetDeviceInfo(device[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
            printf("  Global Memory Size: %llu MB\n", (unsigned long long)mem_size / (1024 * 1024));

            clGetDeviceInfo(device[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
            printf("  Local Memory Size: %llu KB\n", (unsigned long long)mem_size / 1024);

            clGetDeviceInfo(device[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &mem_size, NULL);
            printf("  Max Memory Allocation Size: %llu MB\n", (unsigned long long)mem_size / (1024 * 1024));
            size_t size;
            clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &size, NULL);
            printf("\t\tMax Work-group Total Size: %ld\n", size);
            clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &max_work_item_dimensions, NULL);
            printf("\t\tMax Work-item Dimensions: %u\n", max_work_item_dimensions);
            size_t dims[max_work_item_dimensions];
            clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(dims), &dims, NULL);
            printf("\t\tMax Work-item Sizes: %ld\n", size);
            printf("\t\tMax Work-group Dims: ( ");
            for (size_t k = 0; k < max_work_item_dimensions; k++)
            {
                printf("%ld ", dims[k]);
            }
            printf(")\n");
        }
    }
    return 0;
}

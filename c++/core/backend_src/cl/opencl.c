#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/opencl.h>
#include <CL/cl.h>
#include "vectorinf.h"
#include "opencl_algorithm_lib.h"

extern "C" __attribute__((visibility("default"))) void AABB(uint32_t vecnum, uint32_t objectnum) {
    cl_platform_id platform;
    cl_context context;
    cl_device_id device;
    cl_kernel kernel;
    cl_program program;
    cl_command_queue queue;
    cl_mem triangle_buffer, vpc_buffer, min_buffer, max_buffer, usable_buffer, counter_buffer;
    FILE *kernel_file;
    size_t kernel_file_size, triangle_size, stpvec_size, vec_size, vertex_size, usable_size;
    void *usable_pointer;
    float *stpvec_pointer[3];
    float*vec_pointer[3];
    float*vertex_pointer;
    char *kernel_file_buffer;

    int err;
    err = clGetPlatformIDs(1, &platform, NULL);
    if(err < 0) {
        printf("No platform, sucker");
        return 1;
    }
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if(err < 0) {
        printf("Brother, I even support the gtx 900 series, get a rig ");
        return 2;
    }
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if(err < 0) {
        printf("Couldn't create context");
        return 3;
    }
    kernel_file = fopen("openclc_kernel.cl", "r");
    fseek(kernel_file, 0, SEEK_END);
    kernel_file_size = ftell(kernel_file);
    fseek(kernel_file, 0, SEEK_SET);
    kernel_file_buffer = (char *)malloc(kernel_file_size + 1);
    kernel_file_buffer[kernel_file_size] = '\0';
    fread(kernel_file_buffer, 1,kernel_file_size, kernel_file);
    fclose(kernel_file);
    program = clCreateProgramWithSource(context, 1,&kernel_file_buffer, &kernel_file_size, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    Data d = GetVector(objectnum, 3, 0);
    void* trianglepointer = d.data;
    triangle_size = d.size;
    d = GetVector(objectnum, 4, vecnum);
    stpvec_pointer = d.data;
    stpvec_size = d.size;
    d = GetVector(objectnum, 5,vecnum);
    vec_pointer = d.data;
    vec_size = d.size;
    d = GetVector(objectnum, 2, 0);
    vertex_size = d.size;
    vertex_pointer = d.data;
    d = GetVector(objectnum, 0, 0);
    usable_size = d.size;
    usable_pointer = d.data;
    uint32_t i =0;
    cl_float4 minmax[2];
    cl_uint counter = 0;
    minmax[0] = {min(stpvec_pointer[0] + i, vec_pointer[0] + i), min(stpvec_pointer[1] + i, vec_pointer[1] + i), min(stpvec_pointer[2] + i, vec_pointer[2] + i), 0.f};
    minmax[1] = {max(stpvec_pointer[0] + i, vec_pointer[0] + i), max(stpvec_pointer[1] + i, vec_pointer[1] + i), max(stpvec_pointer[2] + i, vec_pointer[2] + i), 0.f};

    triangle_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, triangle_size * 3 * sizeof(uint32_t), trianglepointer, &err);
    vpc_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, vertex_size, vertex_pointer[0], &err);
    usable_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, triangle_size * sizeof(int), usable_pointer, &err);
    min_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4), &minmax[0], &err);
    max_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4), &minmax[1], &err);
    counter_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(uint), &counter, &err);
    queue = clCreateCommandQueue(context, device, 0, &err);
    kernel = clCreateKernel(program, "AABB", &err);
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &triangle_buffer);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &vpc_buffer);
    err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &usable_buffer);
    err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &max_buffer);
    err = clSetKernelArg(kernel, 6, sizeof(cl_mem), &min_buffer);
    err = clSetKernelArg(kernel, 7, sizeof(uint), &triangle_size);
    err = clSetKernelArg(kernel, 8, sizeof(cl_mem), &counter_buffer);
    size_t triangle_count = triangle_size / (3 * sizeof(uint32_t));
    size_t global_size = triangle_count;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,&global_size,NULL, 0, NULL, NULL);
    d = GetVector(0, 0);
    usable_size = d.size;
    usable_pointer = d.data;
    cl_uint result_count = 0;
    err = clEnqueueReadBuffer(queue, counter_buffer, CL_TRUE, 0,sizeof(cl_uint), &result_count, 0, NULL, NULL);
    err = clEnqueueReadBuffer(queue, usable_buffer, CL_TRUE, 0, result_count*sizeof(int), usable_pointer, 0, NULL, NULL);

    clReleaseKernel(kernel);
    clReleaseMemObject(usable_buffer); // this might change because of moeller trumbore using the same memory space
    clReleaseMemObject(vpc_buffer);
    clReleaseMemObject(triangle_buffer);
    clReleaseMemObject(min_buffer);
    clReleaseMemObject(max_buffer);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
    return EXIT_SUCCESS;
}

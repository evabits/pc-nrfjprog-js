/* Copyright (c) 2015 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H

#include "nan_wrap.h"

#include <functional>
#include <memory>

#include "osfiles.h"
#include "highlevelnrfjprogdll.h"

#include "highlevel_batons.h"
#include "utility/errormessage.h"

class HighLevel : public Nan::ObjectWrap
{
  public:
    static NAN_MODULE_INIT(Init);
    static void initConsts(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);

  private:
    explicit HighLevel();

    static NAN_METHOD(New);

    // Sync methods

    // Async methods
    static NAN_METHOD(GetLibraryVersion);   // Params: callback(error, libraryversion)
    static NAN_METHOD(GetConnectedDevices); // Params: callback(error, connectedDevices)
    static NAN_METHOD(GetSerialNumbers);    // Params: callback(error, serialnumbers)

    static NAN_METHOD(GetDeviceInfo);  // Params: serialnumber, callback(error, deviceinfo)
    static NAN_METHOD(GetProbeInfo);   // Params: serialnumber, callback(error, probeinfo)
    static NAN_METHOD(GetLibraryInfo); // Params: serialnumber, callback(error, libraryinfo)

    static NAN_METHOD(Read);    // Params: serialnumber, address, length, callback(error, data)
    static NAN_METHOD(ReadU32); // Params: serialnumber, address, callback(error, data)

    static NAN_METHOD(Program); // Params: serialnumber, filename, options {verify, chip_erase_mode,
                                // qspi_erase_mode, reset}, callback(progress), callback(error)
    static NAN_METHOD(ProgramDFU); // Params: serialnumber, filename, callback(progress),
                                   // callback(error)
    static NAN_METHOD(ProgramMcuBootDFU); // Params: serialnumber, filename, callback(progress),
                                   // callback(error)
    static NAN_METHOD(ProgramModemUartDFU); // Params: serialnumber, filename, callback(progress),
                                   // callback(error)

    static NAN_METHOD(ReadToFile); // Params: serialnumber, filename, options {readram, readcode,
                                   // readuicr, readqspi}, callback(progress), callback(error)
    static NAN_METHOD(Verify);     // Params: serialnumber, filename, callback(progress),
                                   // callback(error)
    static NAN_METHOD(Erase);      // Params: serialnumber, options {erase_mode, start_address,
                                   // end_address}, callback(progress), callback(error)

    static NAN_METHOD(Recover); // Params: serialnumber, callback(progress), callback(error)
    static NAN_METHOD(Reset);   // Params: serialnumber, callback(error)

    static NAN_METHOD(Write);    // Params: serialnumber, address, dataarray, callback(error)
    static NAN_METHOD(WriteU32); // Params: serialnumber, address, data, callback(error)

    static NAN_METHOD(OpenDevice);  // Params: serialnumber, callback(error)
    static NAN_METHOD(CloseDevice); // Params: serialnumber, callback(error)

    static NAN_METHOD(RttStart); // Params: serialNumber, { location }, callback(error, down, up)
    static NAN_METHOD(RttStop);  // Params: callback(error)

    static NAN_METHOD(RttRead);  // Params: channelIndex, callback(error, data, raw, time)
    static NAN_METHOD(RttWrite); // Params: channelIndex, data, callback(error, writtenlength, time)

    static void CallFunction(Nan::NAN_METHOD_ARGS_TYPE info,
                             const parse_parameters_function_t &parse,
                             const execute_function_t &execute, const return_function_t &ret,
                             const bool hasSerialNumber = false
    );
    static void ExecuteFunction(uv_work_t *req);
    static void ReturnFunction(uv_work_t *req, int a);

    static void init(v8::Local<v8::FunctionTemplate> target);

    static void log(const char *msg);
    static void log(const std::string &msg);
    static void resetLog();

    static void progressCallback(const char *process);
    static void sendProgress(uv_async_t *handle);

    static bool isRttStarted(Probe_handle_t probe);
    static nrfjprogdll_err_t waitForControlBlock(Probe_handle_t probe, bool &isControlBlockFound);
    static nrfjprogdll_err_t getChannelInformation(RTTStartBaton *baton, bool &isChannelInformationAvailable);
    static nrfjprogdll_err_t rttCleanup(Probe_handle_t probe);
};

#endif // __NRFJPROG_H__

/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CMSIS_DRIVER_USBD_WRAPPER_H_
#define CMSIS_DRIVER_USBD_WRAPPER_H_

#include <cmsis-plus/drivers/usb-device.h>

// ----------------------------------------------------------------------------

extern "C"
{
  // Avoid to include <Driver_USBD.h>
  typedef void
  (*ARM_USBD_SignalDeviceEvent_t) (uint32_t event); ///< Pointer to \ref ARM_USBD_SignalDeviceEvent : Signal USB Device Event.
  typedef void
  (*ARM_USBD_SignalEndpointEvent_t) (uint8_t ep_addr, uint32_t event); ///< Pointer to \ref ARM_USBD_SignalEndpointEvent : Signal USB Endpoint Event.

  typedef struct _ARM_DRIVER_USBD const ARM_DRIVER_USBD;
}

namespace os
{
    namespace driver
    {
      // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      // This wrapper makes a CMSIS USBD Keil driver behave like a
      // CMSIS++ Serial driver.

      class Usbd_wrapper : public usb::Device
      {
      public:

        // --------------------------------------------------------------------

        Usbd_wrapper (ARM_DRIVER_USBD* driver,
                      ARM_USBD_SignalDeviceEvent_t c_cb_device_func,
                      ARM_USBD_SignalEndpointEvent_t c_cb_endpoint_func)
                          noexcept;

        Usbd_wrapper (const Usbd_wrapper&) = delete;

        Usbd_wrapper (Usbd_wrapper&&) = delete;

        Usbd_wrapper&
        operator= (const Usbd_wrapper&) = delete;

        Usbd_wrapper&
        operator= (Usbd_wrapper&&) = delete;

        virtual
        ~Usbd_wrapper () noexcept;

        // --------------------------------------------------------------------

      protected:

        virtual const Version&
        do_get_version (void) noexcept override;

        const usb::device::Capabilities&
        do_get_capabilities (void) noexcept override;

        virtual return_t
        do_power (Power state) noexcept override;

        virtual return_t
        do_connect (void) noexcept override;

        virtual return_t
        do_disconnect (void) noexcept override;

        virtual usb::device::Status&
        do_get_status (void) noexcept override;

        virtual return_t
        do_wakeup_remote (void) noexcept override;

        virtual return_t
        do_configure_address (usb::device_address_t dev_addr) noexcept override;

        virtual return_t
        do_read_setup_packet (uint8_t* buf) noexcept override;

        virtual usb::frame_number_t
        do_get_frame_number (void) noexcept override;

        virtual return_t
        do_configure_endpoint (usb::endpoint_t ep_addr,
                               usb::Endpoint_type ep_type,
                               usb::packet_size_t ep_max_packet_size)
                                   noexcept override;

        virtual return_t
        do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept override;

        virtual return_t
        do_stall_endpoint (usb::endpoint_t ep_addr, bool stall)
            noexcept override;

        virtual return_t
        do_transfer (usb::endpoint_t ep_addr, uint8_t* data, std::size_t num)
            noexcept override;

        virtual std::size_t
        do_get_transfer_count (usb::endpoint_t ep_addr) noexcept override;

        virtual return_t
        do_abort_transfer (usb::endpoint_t ep_addr) noexcept override;

        // --------------------------------------------------------------------

      private:

        /// Pointer to CMSIS USBD Keil driver.
        ARM_DRIVER_USBD* driver_;

        /// Pointer to non-reentrant callback. Must be stored because
        /// Initialize() is now delayed just before PowerControl(FULL).
        ARM_USBD_SignalDeviceEvent_t c_cb_device_func_;
        ARM_USBD_SignalEndpointEvent_t c_cb_endpoint_func_;

        // Attempts to somehow use && failed, since the Keil driver
        // functions return temporary objects. So the only portable
        // solution was to copy these objects here and return
        // references to these objects.
        // (Not particularly proud of this solution, but could not find
        // a better one.)

        Version version_
          { 0, 0 };
        usb::device::Capabilities capa_;
        usb::device::Status status_;

      };

#pragma GCC diagnostic pop

    } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* CMSIS_DRIVER_USBD_WRAPPER_H_ */

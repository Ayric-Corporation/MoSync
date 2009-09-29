/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** \file BluetoothDiscovery.h
* \brief \link MAUtil::Environment Environment \endlink-based classes for Bluetooth Discovery.
*/

#ifndef _SE_MSAB_MAUTIL_BLUETOOTHDISCOVERY_H_
#define _SE_MSAB_MAUTIL_BLUETOOTHDISCOVERY_H_

#include <ma.h>
#include "String.h"
#include "Environment.h"

namespace MAUtil {
	/**
	* A C++ representation of a Bluetooth device.
	* \see BluetoothDiscoverer
	*/
	struct BtDevice {
		String name;
		MABtAddr address;	//could be changed to a class type which could have a toString() method.
	};

	/**
	* A C++ representation of a Bluetooth service.
	* \see BluetoothDiscoverer
	*/
	struct BtService {
		int port;
		String name;
		Vector<MAUUID> uuids;
	};

	/**
	* An interface for listening to Bluetooth device discovery events.
	* \see BluetoothDiscoverer
	*/
	class BluetoothDeviceDiscoveryListener {
	public:
		/**
		* Called when a new device is found.
		* The reference becomes invalid when this function returns.
		* Make sure to copy any data you wish to keep.
		*/
		virtual void btNewDevice(const BtDevice& dev) = 0;

		/**
		* Called when the device discovery ends.
		* \param state =\> 0 if successful, or
		* a \link #CONNERR_GENERIC CONNERR \endlink constant on failure.
		*/
		virtual void btDeviceDiscoveryFinished(int state) = 0;
	};

	/**
	* An interface for listening to Bluetooth service discovery events.
	* \see BluetoothDiscoverer
	*/
	class BluetoothServiceDiscoveryListener {
	public:
		/**
		* The reference becomes invalid when this function returns.
		* Make sure to copy any data you wish to keep.
		*/
		virtual void btNewService(const BtService& serv) = 0;

		/**
		* Called when the service discovery is ends.
		* \param state =\> 0 if successful, or
		* a \link #CONNERR_GENERIC CONNERR \endlink constant on failure.
		*/
		virtual void btServiceDiscoveryFinished(int state) = 0;
	};

	/**
	* \brief A class that connects to Environment and handles Bluetooth discovery operations.
	*/
	class BluetoothDiscoverer : private BluetoothListener {
	public:
		BluetoothDiscoverer();
		/**
		* Starts a device discovery operation.
		*
		* Will cause a panic if there is no Environment, or if \a listener is NULL,
		* or if the previous operation has not yet completed.
		*
		* \param listener A pointer to the object that will recieve the events
		* generated by the operation.
		* \param names If true, the operation will fetch the names of the remote devices.
		* This will take extra time. If false, names will not be fetched, speeding up the
		* operation.
		*
		* \returns Zero, if the operation started successfully.
		* Less than zero, if the operation failed. In that case,
		* the operation is considered complete and another may be attempted.
		*/
		int startDeviceDiscovery(BluetoothDeviceDiscoveryListener* listener, bool names);

		/**
		* Starts a service discovery operation.
		*
		* Will cause a panic if there is no Environment, or if \a listener is NULL,
		* or if the previous operation has not yet completed.
		*
		* \param address The address of the device to query.
		* \param uuid The service class UUID to search for.
		* \param listener A pointer to the object that will recieve the events
		* generated by the operation.
		*
		* \returns Zero, if the operation started successfully.
		* Less than zero, if the operation failed. In that case,
		* the operation is considered complete and another may be attempted.
		*/
		int startServiceDiscovery(const MABtAddr& address, const MAUUID& uuid,
			BluetoothServiceDiscoveryListener* listener);
	private:
		BluetoothDeviceDiscoveryListener* mDD;
		BluetoothServiceDiscoveryListener* mSD;

		void bluetoothEvent(int state);
		void handleDD(int state);
		void handleSD(int state);
	};
}

#endif	//_SE_MSAB_MAUTIL_BLUETOOTHDISCOVERY_H_

/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2018 Icinga Development Team (https://www.icinga.com/)  *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef DOWNTIME_H
#define DOWNTIME_H

#include "icinga/i2-icinga.hpp"
#include "icinga/downtime.thpp"
#include "icinga/checkable.thpp"
#include "remote/messageorigin.hpp"

namespace icinga
{

/**
 * A downtime.
 *
 * @ingroup icinga
 */
class Downtime final : public ObjectImpl<Downtime>
{
public:
	DECLARE_OBJECT(Downtime);
	DECLARE_OBJECTNAME(Downtime);

	static boost::signals2::signal<void (const Downtime::Ptr&)> OnDowntimeAdded;
	static boost::signals2::signal<void (const Downtime::Ptr&)> OnDowntimeRemoved;
	static boost::signals2::signal<void (const Downtime::Ptr&)> OnDowntimeStarted;
	static boost::signals2::signal<void (const Downtime::Ptr&)> OnDowntimeTriggered;

	intrusive_ptr<Checkable> GetCheckable() const;

	bool IsInEffect() const;
	bool IsTriggered() const;
	bool IsExpired() const;
	bool HasValidConfigOwner() const;

	static int GetNextDowntimeID();

	static String AddDowntime(const intrusive_ptr<Checkable>& checkable, const String& author,
		const String& comment, double startTime, double endTime, bool fixed,
		const String& triggeredBy, double duration, const String& scheduledDowntime = String(),
		const String& scheduledBy = String(), const String& id = String(),
		const MessageOrigin::Ptr& origin = nullptr);

	static void RemoveDowntime(const String& id, bool cancelled, bool expired = false, const MessageOrigin::Ptr& origin = nullptr);

	void TriggerDowntime();

	static String GetDowntimeIDFromLegacyID(int id);

protected:
	void OnAllConfigLoaded() override;
	void Start(bool runtimeCreated) override;
	void Stop(bool runtimeRemoved) override;

	void ValidateStartTime(const Timestamp& value, const ValidationUtils& utils) override;
	void ValidateEndTime(const Timestamp& value, const ValidationUtils& utils) override;

private:
	ObjectImpl<Checkable>::Ptr m_Checkable;

	bool CanBeTriggered();

	static void DowntimesStartTimerHandler();
	static void DowntimesExpireTimerHandler();
};

}

#endif /* DOWNTIME_H */

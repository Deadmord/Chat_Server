#ifndef QUERYHELPER_H
#define QUERYHELPER_H

#include <QString>

namespace Helper {

	class QueryHelper
	{
	public:
		static QString getAllRooms();
		static QString getAllActiveRooms();
		static QString getRoomById();
		static QString createRoom();
		static QString deleteRoom();
		static QString getUserByLogin();
		static QString createUser();
		static QString updateUserRating();
		static QString getRatingByLogin();
		static QString getTopicIdByTopicName();
	};
}


#endif // !QUERYHELPER_H

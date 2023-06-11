//#include "DTORoom.h"
//
//namespace DTOHelper {
//
//	DTORoom::DTORoom() {};
//
//	DBEntity::DBRoom DTORoom::convertToDBModel(const RoomController& rc_) {
//		return DBEntity::DBRoom(rc_.getId(), rc_.get_name(), rc_.getDescrption(), rc_.getTopicId(), rc_.isPrivate(), rc_.getPassword(), rc_.isDeleted());
//	}
//
//	RoomController DTORoom::convertToEntity(const DBEntity::DBRoom& room_) {
//		QVector<User_Message> empty_messages;
//		QVector<UserConnection*> empty_connected_users;
//		return RoomController(room_.getId(), room_.getName(), room_.getDescription(), room_.getTopicId(), room_.isPrivate(), room_.getPassword(), room_.isDeleted(), empty_messages, empty_connected_users);
//	}
//
//}

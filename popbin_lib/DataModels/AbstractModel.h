#ifndef ABSDATAMODEL_H
#define ABSDATAMODEL_H

#include "../Entry.h"

namespace popbin {

	class AbstractModel {
		public:
			AbstractModel(Entry* entry) { mEntry = entry; }

		protected:
			Entry* mEntry;

	};

}

#endif

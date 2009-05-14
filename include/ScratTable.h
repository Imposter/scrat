//
// ScratTable: Table Binding
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//	1. The origin of this software must not be misrepresented; you must not
//	claim that you wrote the original software. If you use this software
//	in a product, an acknowledgment in the product documentation would be
//	appreciated but is not required.
//
//	2. Altered source versions must be plainly marked as such, and must not be
//	misrepresented as being the original software.
//
//	3. This notice may not be removed or altered from any source
//	distribution.
//

#if !defined(_SCRAT_TABLE_H_)
#define _SCRAT_TABLE_H_

#include <squirrel.h>
#include <string.h>

#include "ScratObject.h"
#include "ScratGlobalMethods.h"

namespace Scrat {

	class Table : public Object {
	public:
		Table(HSQUIRRELVM v) : Object(v, false) {
			sq_newtable(vm);
			sq_getstackobj(vm,-1,&obj);
			sq_pop(vm,-1);
		}

	public:
		// Bind a Table or Class to the Table (Can be used to facilitate Namespaces)
		// Note: Bind cannot be called "inline" like other functions because it introduces order-of-initialization bugs.
		void Bind(const SQChar* name, Object& obj) {
			sq_pushobject(vm, GetObject());
			sq_pushstring(vm, name, -1);
			sq_pushobject(vm, obj.GetObject());
			sq_newslot(vm, -3, false);
			sq_pop(vm,-1); // pop table
		}
		
		// Bind a raw Squirrel closure to the Table
		Table& SquirrelFunc(const SQChar* name, SQFUNCTION func) {
			sq_pushobject(vm, GetObject());
			sq_pushstring(vm, name, -1);
			sq_newclosure(vm, func, 0);
			sq_newslot(vm, -3, false);
			sq_pop(vm,-1); // pop table

			return *this;
		}

		

		//
		// Variable Binding
		//
		
		template<class V>
		Table& SetValue(const SQChar* name, const V& val) {
			BindValue<V>(name, val, false);
			return *this;
		}

		template<class V>
		Table& SetInstance(const SQChar* name, V* val) {
			BindInstance<V>(name, val, false);
			return *this;
		}

		template<class F>
		Table& Func(const SQChar* name, F method) {
			BindFunc(name, &method, sizeof(method), SqGlobalFunc(method));
			return *this;
		}
	};

	//
	// Root Table
	//

	class RootTable : public Table {
	public:
		RootTable(HSQUIRRELVM v) : Table(v) {
			sq_pushroottable(vm);
			sq_getstackobj(vm,-1,&obj);
			sq_pop(v,-1); // pop root table
		}
	};
}

#endif

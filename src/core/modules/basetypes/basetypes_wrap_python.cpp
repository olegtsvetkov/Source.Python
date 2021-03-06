/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "tier0/basetypes.h"
#include "Color.h"
#include "dt_common.h"
#include "dt_send.h"
#include "server_class.h"
#include "modules/memory/memory_tools.h"
#include "utility/sp_util.h"
#include "basetypes_wrap.h"

#include ENGINE_INCLUDE_PATH(basetypes_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_interval();
void export_color();
void export_send_table();
void export_send_prop();
void export_send_prop_types();
void export_send_prop_flags();
void export_server_class();
void export_take_damage_info();
void export_bf_write();
void export_bf_read();


//-----------------------------------------------------------------------------
// Declare the basetypes_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_basetypes)
{
	export_interval();
	export_color();
	export_send_table();
	export_send_prop();
	export_send_prop_types();
	export_send_prop_flags();
	export_server_class();
	export_take_damage_info();
	export_bf_write();
	export_bf_read();
}


//-----------------------------------------------------------------------------
// Expose interval_t.
//-----------------------------------------------------------------------------
void export_interval()
{
	class_<interval_t, interval_t *> Interval("Interval");
	
	// Properties...
	Interval.def_readwrite("start", &interval_t::start);
	Interval.def_readwrite("range", &interval_t::range);
	
	// Add memory tools...
	Interval ADD_MEM_TOOLS(interval_t, "Interval");
}


//-----------------------------------------------------------------------------
// Expose Color.
//-----------------------------------------------------------------------------
class ColorExt
{
public:
	static Color WithAlpha(const Color& color, unsigned char a)
	{
		return Color(color.r(), color.g(), color.b(), a);
	}

	static str GetHexString(const Color& color)
	{
		char buffer[10];
		if (color.a() == 255)
			sprintf(buffer, "\x07%02X%02X%02X", color.r(), color.g(), color.b());
		else
			sprintf(buffer, "\x08%02X%02X%02X%02X", color.r(), color.g(), color.b(), color.a());

		return str(buffer);
	}
};

void export_color()
{
	class_<Color, Color *> Color_(
		"Color",
		init<unsigned char, unsigned char, unsigned char, unsigned char>(
			(arg("r")=0, arg("g")=0, arg("b")=0, arg("a")=255)
		)
	);
	
	// Properties...
	Color_.add_property("r", &Color::r, &IndexSetter<Color, unsigned char, 0>);
	Color_.add_property("g", &Color::g, &IndexSetter<Color, unsigned char, 1>);
	Color_.add_property("b", &Color::b, &IndexSetter<Color, unsigned char, 2>);
	Color_.add_property("a", &Color::a, &IndexSetter<Color, unsigned char, 3>);

	// Methods
	Color_.def(
		"with_alpha",
		ColorExt::WithAlpha,
		"Returns a copy of the color with a new alpha value."
	);

	// Special methods...
	Color_.def("__getitem__", &GetItemIndexer<Color, unsigned char, 0, 3>);
	Color_.def("__setitem__", &SetItemIndexer<Color, unsigned char, 0, 3>);
	Color_.def(
		"__str__",
		&ColorExt::GetHexString,
		"Returns the color as a hex string."
	);

	// Operators...
	Color_.def(self == self);
	Color_.def(self != self);

	// Add memory tools...
	Color_ ADD_MEM_TOOLS(Color, "Color");
}


//-----------------------------------------------------------------------------
// Expose SendTable.
//-----------------------------------------------------------------------------
void export_send_table()
{
	class_<SendTable, SendTable *> SendTable_("SendTable", no_init);
	
	// Properties...
	SendTable_.def_readonly("props", &SendTable::m_pProps);
	SendTable_.def_readonly("length", &SendTable::m_nProps);
	SendTable_.add_property("name", &SendTable::GetName);
	
	// Methods...
	SendTable_.def("get_name", &SendTable::GetName);
	SendTable_.def("get_length", &SendTable::GetNumProps);
	SendTable_.def("is_initialized", &SendTable::IsInitialized);
	SendTable_.def("set_initialized", &SendTable::SetInitialized);
	
	SendTable_.def("get_prop", &SendTable::GetProp,
		reference_existing_object_policy()
	);
	
	SendTable_.def("set_write_flag", &SendTable::SetWriteFlag);
	SendTable_.def("get_write_flag", &SendTable::GetWriteFlag);
	SendTable_.def("has_props_encoded_against_tick_count", &SendTable::HasPropsEncodedAgainstTickCount);
	SendTable_.def("set_has_props_encoded_against_tick_count", &SendTable::SetHasPropsEncodedAgainstTickcount);
	
	// Special methods...
	SendTable_.def("__getitem__",
		&SendTableSharedExt::__getitem__,
		reference_existing_object_policy()
	);
	
	SendTable_.def("__len__", &SendTable::GetNumProps);
	
	// Add memory tools...
	SendTable_ ADD_MEM_TOOLS(SendTable, "SendTable");
}


//-----------------------------------------------------------------------------
// Expose SendProp.
//-----------------------------------------------------------------------------
void export_send_prop()
{
	class_<SendProp, SendProp *> SendProp_("SendProp", no_init);
	
	// Properties...
	SendProp_.def_readonly("type", &SendProp::m_Type);
	SendProp_.def_readonly("bits", &SendProp::m_nBits);
	SendProp_.def_readonly("low_value", &SendProp::m_fLowValue);
	SendProp_.def_readonly("high_value", &SendProp::m_fHighValue);
	SendProp_.def_readonly("array_prop", &SendProp::m_pArrayProp);
	SendProp_.def_readonly("length", &SendProp::m_nElements);
	SendProp_.def_readonly("element_stride", &SendProp::m_ElementStride);
	SendProp_.def_readonly("exclude_data_table_name", &SendProp::m_pExcludeDTName);
	SendProp_.def_readonly("parent_array_prop_name", &SendProp::m_pParentArrayPropName);
	SendProp_.add_property("name", &SendProp::GetName);
	SendProp_.def_readonly("high_low_mul", &SendProp::m_fHighLowMul);
	SendProp_.add_property("flags", &SendProp::GetFlags);
	
	SendProp_.add_property("data_table",
		make_function(
			&SendProp::GetDataTable,
			reference_existing_object_policy()
		)
	);
	
	SendProp_.add_property("offset", &SendProp::GetOffset);
	
	// CS:GO specific properties...
	SendProp_.NOT_IMPLEMENTED_ATTR("priority");
	
	// Methods...
	SendProp_.def("get_offset", &SendProp::GetOffset);
	
	SendProp_.def("get_data_table",
		&SendProp::GetDataTable,
		reference_existing_object_policy()
	);

	SendProp_.def("get_exclude_data_table_name", &SendProp::GetExcludeDTName);
	SendProp_.def("get_parent_array_prop_name", &SendProp::GetParentArrayPropName);
	SendProp_.def("get_name", &SendProp::GetName);
	SendProp_.def("is_signed", &SendProp::IsSigned);
	SendProp_.def("is_exclude_prop", &SendProp::IsExcludeProp);
	SendProp_.def("is_inside_array", &SendProp::IsInsideArray);
	
	SendProp_.def("get_array_prop",
		&SendProp::GetArrayProp,
		reference_existing_object_policy()
	);

	SendProp_.def("get_length", &SendProp::GetNumElements);
	SendProp_.def("get_element_stride", &SendProp::GetElementStride);
	SendProp_.def("get_type", &SendProp::GetType);
	SendProp_.def("get_flags", &SendProp::GetFlags);
	
	// CS:GO specific methods...
	SendProp_.NOT_IMPLEMENTED("get_priority");
	
	// Engine specific stuff...
	export_engine_specific_send_prop(SendProp_);
	
	// Add memory tools...
	SendProp_ ADD_MEM_TOOLS(SendProp, "SendProp");
}


//-----------------------------------------------------------------------------
// Expose SendPropType.
//-----------------------------------------------------------------------------
void export_send_prop_types()
{
	enum_<SendPropType> SendPropTypes("SendPropTypes");
	
	// Values...
	SendPropTypes.value("INT", DPT_Int);
	SendPropTypes.value("FLOAT", DPT_Float);
	SendPropTypes.value("VECTOR", DPT_Vector);
	SendPropTypes.value("VECTORXY", DPT_VectorXY);
	SendPropTypes.value("STRING", DPT_String);
	SendPropTypes.value("ARRAY", DPT_Array);
	SendPropTypes.value("DATATABLE", DPT_DataTable);
	
	// CS:GO specific values...
	NOT_IMPLEMENTED_VALUE(SendPropType, "INT64");
	
	// Engine specific stuff...
	export_engine_specific_send_prop_types(SendPropTypes);
}


//-----------------------------------------------------------------------------
// Expose SendPropFlags.
//-----------------------------------------------------------------------------
void export_send_prop_flags()
{
	enum_<SendPropSharedExt::SendPropFlags> SendPropFlags("SendPropFlags");
	
	// Values...
	SendPropFlags.value("UNSIGNED", SendPropSharedExt::UNSIGNED);
	SendPropFlags.value("COORD", SendPropSharedExt::COORD);
	SendPropFlags.value("NO_SCALE", SendPropSharedExt::NO_SCALE);
	SendPropFlags.value("ROUND_DOWN", SendPropSharedExt::ROUND_DOWN);
	SendPropFlags.value("ROUND_UP", SendPropSharedExt::ROUND_UP);
	SendPropFlags.value("NORMAL", SendPropSharedExt::NORMAL);
	SendPropFlags.value("EXCLUDE", SendPropSharedExt::EXCLUDE);
	SendPropFlags.value("XYZE", SendPropSharedExt::XYZE);
	SendPropFlags.value("INSIDE_ARRAY", SendPropSharedExt::INSIDE_ARRAY);
	SendPropFlags.value("PROXY_ALWAYS_YES", SendPropSharedExt::PROXY_ALWAYS_YES);
	SendPropFlags.value("IS_A_VECTOR_ELEM", SendPropSharedExt::IS_A_VECTOR_ELEM);
	SendPropFlags.value("COLLAPSIBLE", SendPropSharedExt::COLLAPSIBLE);
	SendPropFlags.value("COORD_MP", SendPropSharedExt::COORD_MP);
	SendPropFlags.value("COORD_MP_LOW_PRECISION", SendPropSharedExt::COORD_MP_LOW_PRECISION);
	SendPropFlags.value("COORD_MP_INTEGRAL", SendPropSharedExt::COORD_MP_INTEGRAL);
	SendPropFlags.value("CHANGE_OFTEN", SendPropSharedExt::CHANGE_OFTEN);
}


//-----------------------------------------------------------------------------
// Expose ServerClass.
//-----------------------------------------------------------------------------
void export_server_class()
{
	class_<ServerClass, ServerClass *> ServerClass_("ServerClass", no_init);
	
	// Properties...
	ServerClass_.def_readonly("table", &ServerClass::m_pTable);
	ServerClass_.def_readonly("next", &ServerClass::m_pNext);
	ServerClass_.def_readonly("class_index", &ServerClass::m_ClassID);
	
	// Engine specific stuff...
	export_engine_specific_server_class(ServerClass_);
	
	// Add memory tools...
	ServerClass_ ADD_MEM_TOOLS(ServerClass, "ServerClass");
}


//-----------------------------------------------------------------------------
// Expose CTakeDamageInfo.
//-----------------------------------------------------------------------------
void export_take_damage_info()
{
	class_<CTakeDamageInfo, CTakeDamageInfo *> TakeDamageInfo("TakeDamageInfo");
	
	// Initializers...
	TakeDamageInfo.def("__init__", make_constructor(&TakeDamageInfoSharedExt::__init__));
	
	// Properties...
	TakeDamageInfo.add_property("force", &CTakeDamageInfo::GetDamageForce, &CTakeDamageInfo::SetDamageForce);
	TakeDamageInfo.add_property("position", &CTakeDamageInfo::GetDamagePosition, &CTakeDamageInfo::SetDamagePosition);
	
	TakeDamageInfo.add_property("reported_position", &CTakeDamageInfo::GetReportedPosition,
		&CTakeDamageInfo::SetReportedPosition
	);
	
	TakeDamageInfo.add_property("damage", &CTakeDamageInfo::GetDamage, &CTakeDamageInfo::SetDamage);
	TakeDamageInfo.add_property("base_damage", &CTakeDamageInfo::GetBaseDamage, &TakeDamageInfoSharedExt::set_base_damage);
	TakeDamageInfo.add_property("type", &CTakeDamageInfo::GetDamageType, &CTakeDamageInfo::SetDamageType);
	TakeDamageInfo.add_property("stats", &CTakeDamageInfo::GetDamageStats, &CTakeDamageInfo::SetDamageStats);
	TakeDamageInfo.add_property("ammo", &CTakeDamageInfo::GetAmmoType, &CTakeDamageInfo::SetAmmoType);
	
	TakeDamageInfo.add_property("damaged_other_players", &TakeDamageInfoSharedExt::get_damaged_other_players,
		&TakeDamageInfoSharedExt::set_damaged_other_players
	);

	TakeDamageInfo.add_property("inflictor", &TakeDamageInfoSharedExt::get_inflictor, &TakeDamageInfoSharedExt::set_inflictor);
	TakeDamageInfo.add_property("attacker", &TakeDamageInfoSharedExt::get_attacker, &TakeDamageInfoSharedExt::set_attacker);
	TakeDamageInfo.add_property("weapon", &TakeDamageInfoSharedExt::get_weapon, &TakeDamageInfoSharedExt::set_weapon);
	
	// CS:GO properties...
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("radius");
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("bullet");
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("recoil");
	
	// Engine specific stuff...
	export_engine_specific_take_damage_info(TakeDamageInfo);
	
	// Add memory tools...
	TakeDamageInfo ADD_MEM_TOOLS(CTakeDamageInfo, "TakeDamageInfo");
}


//-----------------------------------------------------------------------------
// Expose bf_read/bf_write.
//-----------------------------------------------------------------------------
class bf_writeExt {
public:
	static void __del__(bf_write* buffer)
	{
		delete buffer->GetData();
	}

	static boost::shared_ptr<bf_write> __init__(int buffer_size)
	{
		return boost::shared_ptr<bf_write>(new bf_write(new unsigned char[buffer_size], buffer_size), &__del__);
	}

	static list GetData(bf_write& buffer)
	{
		list result;

		unsigned char* data = buffer.GetData();
		for (int i=0; i < buffer.GetNumBytesWritten(); i++) {
			result.append(data[i]);
		}

		return result;
	}
};

void export_bf_write()
{
	class_<bf_write>("bf_write", no_init)
		.def("__init__",
			make_constructor(&bf_writeExt::__init__, default_call_policies())
		)

		.def("seek_to_bit",
			&bf_write::SeekToBit,
			"Seeks to a specific position."
		)

		.def("write_one_bit",
			&bf_write::WriteOneBit
		)

		.def("write_one_bit_no_check",
			&bf_write::WriteOneBitNoCheck
		)

		.def("write_one_bit_at",
			&bf_write::WriteOneBitAt
		)

		.def("write_ubit_long",
			&bf_write::WriteUBitLong,
			("data", "num_bits", arg("check_range")=true)
		)

		.def("write_sbit_long",
			&bf_write::WriteSBitLong
		)

		.def("write_ubit_var",
			&bf_write::WriteUBitVar,
			"Writes an unsigned integer with variable bit length."
		)

		.def("write_bits_from_buffer",
			&bf_write::WriteBitsFromBuffer,
			"Copy the bits straight out of <pIn>. This seeks <pIn> forward by <nBits>. Returns an error if this buffer or the read buffer overflows."
		)

		.def("write_bit_angle",
			&bf_write::WriteBitAngle
		)

		.def("write_bit_coord",
			&bf_write::WriteBitCoord
		)

		.def("write_bit_coord_mp",
			&bf_write::WriteBitCoordMP
		)

		.def("write_bit_float",
			&bf_write::WriteBitFloat
		)

		.def("write_bit_vec3_coord",
			&bf_write::WriteBitVec3Coord
		)

		.def("write_bit_normal",
			&bf_write::WriteBitNormal
		)

		.def("write_bit_vec3_normal",
			&bf_write::WriteBitVec3Normal
		)

		.def("write_bit_angles",
			&bf_write::WriteBitAngles
		)

		.def("write_char",
			&bf_write::WriteChar
		)

		.def("write_byte",
			&bf_write::WriteByte
		)

		.def("write_short",
			&bf_write::WriteShort
		)

		.def("write_word",
			&bf_write::WriteWord
		)

		.def("write_long",
			&bf_write::WriteLong
		)

		.def("write_long_long",
			&bf_write::WriteLongLong
		)

		.def("write_float",
			&bf_write::WriteFloat
		)

		.def("write_string",
			// Required for CS:GO
			GET_METHOD(bool, bf_write, WriteString, const char*),
			"Returns false if it overflows the buffer."
		)

		.def("get_num_bytes_written",
			&bf_write::GetNumBytesWritten
		)

		.def("get_num_bits_written",
			&bf_write::GetNumBitsWritten
		)

		.def("get_max_num_bits",
			&bf_write::GetMaxNumBits
		)

		.def("get_num_bits_left",
			&bf_write::GetNumBitsLeft
		)

		.def("get_num_bytes_left",
			&bf_write::GetNumBytesLeft
		)

		.def("get_data",
			&bf_writeExt::GetData
		)

		.def("check_for_overflow",
			&bf_write::CheckForOverflow
		)

		.def("is_overflowed",
			&bf_write::IsOverflowed
		)

		.def("set_overflow_flag",
			&bf_write::SetOverflowFlag
		)
		
		.def_readwrite("data_bytes_count",
			&bf_write::m_nDataBytes
		)

		.def_readwrite("data_bits_count",
			&bf_write::m_nDataBits
		)

		.def_readwrite("current_bit",
			&bf_write::m_iCurBit
		)

		ADD_MEM_TOOLS(bf_write, "bf_write")
	;
}

class bf_readExt {
public:
	static void __del__(bf_read* buffer)
	{
		delete buffer->GetBasePointer();
	}

	static boost::shared_ptr<bf_read> __init__(bf_write& buffer)
	{
		int size = buffer.GetNumBytesWritten();
		void* pData = new unsigned char[size];
		memcpy(pData, buffer.GetData(), size);
		return boost::shared_ptr<bf_read>(new bf_read(pData, size), &__del__);
	}

	static list GetData(bf_read& buffer)
	{
		list result;
		
		const unsigned char* data = buffer.GetBasePointer();
		for (unsigned int i=0; i < buffer.m_nDataBytes; i++) {
			result.append(data[i]);
		}

		return result;
	}

	static int GetNumBytesRead(bf_read& buffer)
	{
		return BitByte(buffer.GetNumBitsRead());
	}

	static str ReadString(bf_read& buffer)
	{
		char* pStr = new char[buffer.m_nDataBytes];
		buffer.ReadString(pStr, buffer.m_nDataBytes);

		// Let Boost handle deallocating the string
		str result = str((const char *) pStr);
		delete pStr;
		return result;
	}
};

void export_bf_read()
{
	class_<bf_read>("bf_read", no_init)
		.def("__init__",
			make_constructor(&bf_readExt::__init__, default_call_policies())
		)

		.def("read_one_bit",
			&bf_read::ReadOneBit
		)

		.def("read_bit_angle",
			&bf_read::ReadBitAngle
		)

		.def("read_ubit_long",
			&bf_read::ReadUBitLong
		)

		.def("peek_ubit_long",
			&bf_read::PeekUBitLong
		)

		.def("read_sbit_long",
			&bf_read::ReadSBitLong
		)

		.def("read_ubit_var",
			&bf_read::ReadUBitVar
		)

		.def("read_bit_coord",
			&bf_read::ReadBitCoord
		)

		.def("read_bit_coord_mp",
			&bf_read::ReadBitCoordMP
		)

		.def("read_bit_float",
			&bf_read::ReadBitFloat
		)

		.def("read_bit_normal",
			&bf_read::ReadBitNormal
		)

		.def("read_bit_vec3_coord",
			&bf_read::ReadBitVec3Coord
		)

		.def("read_bit_vec3_normal",
			&bf_read::ReadBitVec3Normal
		)

		.def("read_bit_angles",
			&bf_read::ReadBitAngles
		)

		.def("read_char",
			&bf_read::ReadChar
		)

		.def("read_byte",
			&bf_read::ReadByte
		)

		.def("read_short",
			&bf_read::ReadShort
		)

		.def("read_word",
			&bf_read::ReadWord
		)

		.def("read_long",
			&bf_read::ReadLong
		)

		.def("read_long_long",
			&bf_read::ReadLongLong
		)

		.def("read_float",
			&bf_read::ReadFloat
		)
		
		.def("read_string",
			&bf_readExt::ReadString
		)

		.def("get_num_bytes_left",
			&bf_read::GetNumBytesLeft
		)

		.def("get_num_bytes_read",
			&bf_readExt::GetNumBytesRead
		)

		.def("get_num_bits_left",
			&bf_read::GetNumBitsLeft
		)

		.def("get_num_bits_read",
			&bf_read::GetNumBitsRead
		)

		.def("is_overflowed",
			&bf_read::IsOverflowed
		)

		.def("seek",
			&bf_read::Seek
		)

		.def("seek_relative",
			&bf_read::SeekRelative
		)

		.def("set_overflow_flag",
			&bf_read::SetOverflowFlag
		)

		.def("get_data",
			&bf_readExt::GetData
		)

		.def_readwrite("data_bytes_count",
			&bf_read::m_nDataBytes
		)

		.def_readwrite("data_bits_count",
			&bf_read::m_nDataBits
		)
		
		.add_property("current_bit",
			&bf_read::Seek,
			&bf_read::GetNumBitsRead
		)

		ADD_MEM_TOOLS(bf_read, "bf_read")
	;
}

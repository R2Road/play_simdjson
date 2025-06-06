#include "test_element.h"

#include "r2tm/r2tm_Inspector.h"
#include "r2tm/r2tm_ostream.h"

#include "simdjson/simdjson.h"

namespace test_element
{
	r2tm::TitleFunctionT Element_Get::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Element : Get";
		};
	}
	r2tm::DoFunctionT Element_Get::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const auto s = R"( [1] )"_padded );
			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( s ) );

			LS();

			{
				OUTPUT_SUBJECT( "모든 Get은 simdjson_result 를 반환한다." );
				OUTPUT_SUBJECT( "std::pair 기반" );
				OUTPUT_SUBJECT( "error code + data" );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, datas.get_array().error() );

				LF();

				EXPECT_EQ( simdjson::error_code::INCORRECT_TYPE, datas.get_object().error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "type을 지정하면 data만 빼낼 수 있다." );

				LF();

				DECLARATION_MAIN( simdjson::dom::array a = datas.get_array() );
				a.at( 0 );

				LF();

				OUTPUT_COMMENT( "type이 다르면 안된다." );
				OUTPUT_SOURCE_READY_N_BEGIN;
				// simdjson::dom::object o = datas.get_object();
				OUTPUT_SOURCE_END;
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Element_Array::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Element : Array";
		};
	}
	r2tm::DoFunctionT Element_Array::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "Array" );

			LS();

			DECLARATION_MAIN( const auto s = R"( [0, 1, 2, 3] )"_padded );
			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( s ) );

			LS();

			DECLARATION_MAIN( simdjson::dom::array a = datas.get_array() );
			EXPECT_EQ( 4, a.size() );

			LS();

			{
				OUTPUT_SUBJECT( "[] 안된다." );

				LF();

				OUTPUT_SOURCE_READY_N_BEGIN;
				// a[1];
				OUTPUT_SOURCE_END;
			}

			LS();

			{
				OUTPUT_SUBJECT( "at" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element at_3 = a.at( 3 ) );
				EXPECT_EQ( 3, at_3.get_int64() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "ranged-for" );

				LF();

				OUTPUT_SOURCE_READY_N_BEGIN;
				for( const auto i : a ) { std::cout << i << "   "; }
				LF();
				OUTPUT_SOURCE_END;
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Element_Object::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Element : Object";
		};
	}
	r2tm::DoFunctionT Element_Object::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const auto s = R"( { "a" : 123, "b" : [0,1,2] } )"_padded );
			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( s ) );

			LS();

			DECLARATION_MAIN( simdjson::dom::object o = datas.get_object() );
			EXPECT_EQ( 2, o.size() );

			LS();

			{
				OUTPUT_SUBJECT( "[] 된다." );

				LF();

				EXPECT_EQ( 123, o["a"].get_int64() );

				LF();

				EXPECT_EQ( simdjson::error_code::NO_SUCH_FIELD, o["c"].error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "at_key" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element at_key_a = o.at_key( "a" ) );
				EXPECT_EQ( 123, at_key_a.get_int64() );

				LF();

				EXPECT_EQ( simdjson::error_code::NO_SUCH_FIELD, o.at_key( "c" ).error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "ranged-for" );

				LF();

				OUTPUT_SOURCE_READY_N_BEGIN;
				for( const auto& i : o ) { std::cout << i.key << " : " << i.value << "   "; }
				LF();
				OUTPUT_SOURCE_END;
			}

			LS();

			{
				OUTPUT_SUBJECT( "at_path" );

				LF();

				OUTPUT_COMMENT( "JsonPath 학습이 필요." );
				OUTPUT_COMMENT( "https://github.com/json-path/JsonPath" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element at_path_b_2 = o.at_path( "$.b[2]" ));
				EXPECT_EQ( 2, at_path_b_2.get_int64() );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Element_Int::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Element : Int";
		};
	}
	r2tm::DoFunctionT Element_Int::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const auto s = R"( [123] )"_padded );
			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( s ) );

			LS();

			DECLARATION_MAIN( const simdjson::dom::array a = datas.get_array() );

			LS();

			DECLARATION_MAIN( const auto i = a.at( 0 ) );
			EXPECT_EQ( simdjson::error_code::SUCCESS, i.error() );

			LS();

			{
				OUTPUT_SUBJECT( "따로 int 타입이 있는 것이 아니다." );
				OUTPUT_SUBJECT( "simdjson_result<int64_t> 반환" );

				LF();

				EXPECT_EQ( 123, i.get_int64().take_value() );
				EXPECT_EQ( 123, i.get_int64() );
				EXPECT_EQ( 123, int64_t( i ) );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}
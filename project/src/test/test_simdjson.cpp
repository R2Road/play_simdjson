#include "test_simdjson.h"

#include "r2tm/r2tm_Inspector.h"
#include "r2tm/r2tm_ostream.h"

#include "simdjson/simdjson.h"

namespace test_simdjson
{
	r2tm::TitleFunctionT Basic::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "SimdJson : Basic";
		};
	}
	r2tm::DoFunctionT Basic::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( simdjson::dom::parser parser );

			LS();

			DECLARATION_MAIN( const auto abstract_json = R"(
				[
					{  "12345" : {"a":12.34, "b":56.78, "c": 9998877}   },
					{  "12545" : {"a":11.44, "b":12.78, "c": 11111111}  }
				]
			)"_padded );

			LS();

			{
				OUTPUT_FILE_RANGE( __FILE__, 41, 52 );

				SS();

				for( simdjson::dom::object obj : parser.parse( abstract_json ) )
				{
					for( const auto& key_value : obj )
					{
						std::cout << "key: " << key_value.key << " : ";

						simdjson::dom::object innerobj = key_value.value;
						std::cout << "a: " << double( innerobj["a"] ) << ", ";
						std::cout << "b: " << double( innerobj["b"] ) << ", ";
						std::cout << "c: " << int64_t( innerobj["c"] ) << std::endl;
					}
				}
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT ErrorCode::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Error Code";
		};
	}
	r2tm::DoFunctionT ErrorCode::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "Load, Parse 등의 결과를 simdjson::error_code 로 확인 한다." );
			OUTPUT_SUBJECT( "simdjson::error_code는 대응하는 operator<<가 존재 한다." );
			OUTPUT_SUBJECT( "미리 준비된 error message 를 확인 가능하다." );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser parser );

			LS();

			{
				OUTPUT_SUBJECT( "빈 문자열" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string abstract_json = R"()"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::EMPTY, error );
				OUTPUT_VALUE( simdjson::error_code::EMPTY );
			}

			LS();

			{
				OUTPUT_SUBJECT( "완료 되지 않은 데이터 표현" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string abstract_json = R"( [ )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, error );
				OUTPUT_VALUE( simdjson::error_code::TAPE_ERROR );
			}

			LS();

			{
				OUTPUT_SUBJECT( "정상 데이터" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string abstract_json = R"( [] )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).error() );

				LF();

				EXPECT_EQ( error, simdjson::error_code::SUCCESS );
				OUTPUT_VALUE( simdjson::error_code::SUCCESS );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Parse_String::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Parse : String";
		};
	}
	r2tm::DoFunctionT Parse_String::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const simdjson::padded_string jsong_string = R"( [1, 2, 3, 4] )"_padded );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "결과 데이터 획득 : 데이터 + error code" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( jsong_string ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				PROCESS_MAIN( result.get( datas ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "잘못된 데이터" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( R"( [ )"_padded ) );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, result.error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "데이터 획득" );

				LF();

				DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( jsong_string ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "에러 메세지와 데이터 획득" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				DECLARATION_MAIN( const simdjson::error_code error = p.parse( jsong_string ).get( datas ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, error );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}






	r2tm::TitleFunctionT PaddedString::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Padded String";
		};
	}
	r2tm::DoFunctionT PaddedString::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "parser의 작동을 용이하게 해주는 문자열 타입" );
			OUTPUT_SUBJECT( "...이란다." );

			LS();

			{
				OUTPUT_SUBJECT( "기본 생성자" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string s );

				LF();

				EXPECT_EQ( 0, s.length() );
				EXPECT_EQ( 0, s.size() );
				EXPECT_EQ( nullptr, s.data() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "char*" );

				LF();

				DECLARATION_MAIN( const char* c = "123" );
				DECLARATION_MAIN( const simdjson::padded_string s( c, strlen( c ) ) );

				LF();

				EXPECT_EQ( strlen( c ), s.length() );
				EXPECT_EQ( strlen( c ), s.size() );
				EXPECT_NE( c, s.data() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "File" );

				LF();

				DECLARATION_MAIN( const char* f = "resource/json_from_plp.json" );

				SS();

				DECLARATION_MAIN( simdjson::simdjson_result result = simdjson::padded_string::load( f ) );
				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				SS();

				DECLARATION_MAIN( simdjson::padded_string s = simdjson::padded_string::load( f ) );
				OUTPUT_VALUE( s );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Parse_File::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Parse : File";
		};
	}
	r2tm::DoFunctionT Parse_File::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const char* const path = "resource/json_from_plp.json" );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "결과 데이터 획득 : 데이터 + error code" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.load( path ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				PROCESS_MAIN( result.get( datas ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "데이터 획득" );

				LF();

				DECLARATION_MAIN( const simdjson::dom::element datas = p.load( path ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "에러 메세지와 데이터 획득" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				DECLARATION_MAIN( const simdjson::error_code error = p.load( path ).get( datas ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, error );
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

				LF();

				OUTPUT_COMMENT( "element 에서 바로 형변환." );
				EXPECT_EQ( 3, int64_t( a.at( 3 ) ) );
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



	r2tm::TitleFunctionT Parse_Iterate::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Parse : Iterate";
		};
	}
	r2tm::DoFunctionT Parse_Iterate::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const char* const path = "resource/json_from_plp.json" );

			SS();

			DECLARATION_MAIN( simdjson::padded_string s = simdjson::padded_string::load( path ) );

			LS();

			DECLARATION_MAIN( simdjson::fallback::ondemand::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "결과 데이터 획득 : 데이터 + error code" );

				LF();

				DECLARATION_MAIN( simdjson::simdjson_result result = p.iterate( s ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				OUTPUT_NOTE( "데이터 꺼내는 법을 모르겠어" );
				OUTPUT_NOTE( "안되겠다 일단 후퇴. 2025.06.06" );
			}

			LS();

			{
				OUTPUT_SUBJECT( "데이터 획득" );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::document doc = p.iterate( s ) );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::array ary = doc.get_array() );
				DECLARATION_MAIN( simdjson::fallback::ondemand::object obj = ary.at( 0 ) );
				OUTPUT_VALUE( obj["idx"] );
			}

			LS();

			{
				OUTPUT_SUBJECT( "에러 메세지와 데이터 획득" );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::document doc );
				DECLARATION_MAIN( simdjson::error_code error = p.iterate( s ).get( doc ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, error );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::array ary = doc.get_array() );
				DECLARATION_MAIN( simdjson::fallback::ondemand::object obj = ary.at( 0 ) );
				OUTPUT_VALUE( obj["idx"] );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}
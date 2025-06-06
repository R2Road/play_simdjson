#include "test_basic.h"

#include "r2tm/r2tm_Inspector.h"
#include "r2tm/r2tm_ostream.h"

#include "simdjson/simdjson.h"

namespace test_basic
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



	r2tm::TitleFunctionT ResultType::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "ResultType";
		};
	}
	r2tm::DoFunctionT ResultType::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "대부분의 경우 simdjson 의 메소드들은 simdjson_result 를 반환한다" );
			OUTPUT_SUBJECT( "std::pair 기반" );
			OUTPUT_SUBJECT( "error code + data" );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( R"( [1] )"_padded ) );

			LS();

			{
				OUTPUT_SUBJECT( "simdjson_result::error() : 에러 코드 확보" );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "simdjson_result::get() : 값 추출 + 에러 코드 반환" );

				LF();

				DECLARATION_MAIN( simdjson::dom::array a );
				EXPECT_EQ( simdjson::error_code::SUCCESS, result.get( a ) );

				LF();

				EXPECT_EQ( 1, a.size() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "simdjson_result 가 필요 없다면 바로 값 추출" );

				LF();

				DECLARATION_MAIN( simdjson::dom::array a = p.parse( R"( [1] )"_padded ) );

				LF();

				EXPECT_EQ( 1, a.size() );
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

			OUTPUT_SUBJECT( "메소드의 작동 결과를 simdjson::error_code 로 확인." );
			OUTPUT_SUBJECT( "simdjson::error_code는 대응하는 operator<< 가 존재 한다." );
			OUTPUT_SUBJECT( "미리 준비된 error message 를 확인 가능하다." );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser parser );

			LS();

			{
				OUTPUT_SUBJECT( "빈 문자열" );

				LF();

				DECLARATION_MAIN( const auto s = R"()"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code e = parser.parse( s ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::EMPTY, e );
				OUTPUT_VALUE( simdjson::error_code::EMPTY );
			}

			LS();

			{
				OUTPUT_SUBJECT( "완료 되지 않은 데이터 표현" );

				LF();

				DECLARATION_MAIN( const auto s = R"( [ )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code e = parser.parse( s ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, e );
				OUTPUT_VALUE( simdjson::error_code::TAPE_ERROR );
			}

			LS();

			{
				OUTPUT_SUBJECT( "정상 데이터" );

				LF();

				DECLARATION_MAIN( const auto s = R"( [] )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code e = parser.parse( s ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, e );
				OUTPUT_VALUE( simdjson::error_code::SUCCESS );
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
				OUTPUT_SUBJECT( "operator \"\"_padded" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string s = R"( [] )"_padded );

				LF();

				EXPECT_NE( nullptr, s.data() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "File" );

				LF();

				DECLARATION_MAIN( const char* f = "resource/json_basic.json" );
				DECLARATION_MAIN( simdjson::simdjson_result result = simdjson::padded_string::load( f ) );
				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				OUTPUT_VALUE( result.value() );
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
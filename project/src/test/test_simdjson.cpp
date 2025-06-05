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

			OUTPUT_COMMENT( "Load, Parse 등의 결과를 simdjson::error_code 로 확인 한다." );
			OUTPUT_COMMENT( "simdjson::error_code는 대응하는 operator<<가 존재 한다." );
			OUTPUT_COMMENT( "미리 준비된 error message 를 확인 가능하다." );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser parser );
			DECLARATION_MAIN( simdjson::dom::element datas );

			LS();

			{
				OUTPUT_SUBJECT( "빈 문자열" );

				LF();

				DECLARATION_MAIN( const auto abstract_json = R"()"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).get( datas ) );

				LF();

				EXPECT_EQ( simdjson::error_code::EMPTY, error );
				OUTPUT_VALUE( simdjson::error_code::EMPTY );
			}

			LS();

			{
				OUTPUT_SUBJECT( "완료 되지 않은 데이터 표현" );

				LF();

				DECLARATION_MAIN( const auto abstract_json = R"( [ )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).get( datas ) );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, error );
				OUTPUT_VALUE( simdjson::error_code::TAPE_ERROR );
			}

			LS();

			{
				OUTPUT_SUBJECT( "정상 데이터" );

				LF();

				DECLARATION_MAIN( const auto abstract_json = R"( [] )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).get( datas ) );

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

			DECLARATION_MAIN( simdjson::dom::parser parser );
			DECLARATION_MAIN( simdjson::dom::element datas );

			LS();

			{
				OUTPUT_SUBJECT( "데이터 획득" );

				LF();

				DECLARATION_MAIN( const auto abstract_json = R"( [1, 2, 3, 4] )"_padded );

				LF();

				PROCESS_MAIN( datas = parser.parse( abstract_json ) );
			}

			LS();

			{
				OUTPUT_SUBJECT( "에러 메세지와 데이터 획득" );

				LF();

				DECLARATION_MAIN( const auto abstract_json = R"( [1, 2, 3, 4] )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code error = parser.parse( abstract_json ).get( datas ) );

				LF();

				EXPECT_EQ( error, simdjson::error_code::SUCCESS );
				OUTPUT_VALUE( simdjson::error_code::SUCCESS );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}
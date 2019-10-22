// SqliteTestApp.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <mysqlsession/Session.h>
#include <Dal.h>
#include <Schema.h>
#include <DataSchema.h>
#include <TransactionPatch.h>
#include <PatchFactory.h>
#include <JsonReader.h>


void selectJournal(repo::ISession& s)
{
	std::stringstream selectJournal;
	selectJournal << "select * from " << repo::ProductSchema::private_Journal::TableName << " order by id;";
	std::cout << "Journal" << std::endl;
	s.ExecSql(selectJournal.str().c_str(), [](const repo::IRow& row) {
		const repo::ProductSchema::private_Journal t(row);
		std::cout << "------------------------" << std::endl;
		std::cout << t.Get_id() << ": " << t.Get_schemaVersion().Data() << ": " << t.Get_time() << ": " << t.Get_patchJson().Data() << std::endl;
		});
}

int main(int argc, char* argv[])
{
	try {
		repo::schema::RegisterSchema();
		repo::mysql::Session s = repo::mysql::Session::Get("localhost", "root", "1111", "test_db", 3306);
		repo::schema::ProductSchema.UpdateSchema(s);

		selectJournal(s);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}


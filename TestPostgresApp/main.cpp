// SqliteTestApp.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <postgresession/Session.h>
#include <Dal.h>
#include <Schema.h>
#include <DataSchemaDeploy.h>
#include <TransactionPatch.h>
#include <PatchFactory.h>
#include <JsonReader.h>


void selectJournal(repo::ISession& s)
{
	std::stringstream selectJournal;
	selectJournal << "select * from " << repo::dal::private_Journal::TableName << " order by id;";
	std::cout << "Journal" << std::endl;
	s.ExecSql(selectJournal.str().c_str(), [](const repo::IRow& row) {
		const repo::dal::private_Journal t(row);
		std::cout << "------------------------" << std::endl;
		std::cout << t.Get_id() << ": " << t.Get_time() << ": " << t.Get_patchJson().Data() << std::endl;
		});
}

int main(int argc, char* argv[])
{
	try {
		repo::schema::RegisterSchema();
		repo::postgresql::Session s = repo::postgresql::Session::Get("user=postgres password=1 host=127.0.0.1 port=5432 dbname=test_db");
		repo::schema::ProductSchema.UpdateSchema(s);

		selectJournal(s);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}


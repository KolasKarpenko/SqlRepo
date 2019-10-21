#include "sqlitesession/ZeroSchemaPatch.h"

#include <sstream>
#include <PatchFactory.h>

namespace repo
{
namespace sqlite
{

ZeroSchemaPatch::ZeroSchemaPatch() : repo::Patch(201812242313)
{
}

std::string ZeroSchemaPatch::GetType() const
{
	return typeid(ZeroSchemaPatch).name();
}

void ZeroSchemaPatch::ToJsonImpl(Json::Value& /*json*/) const
{
}

void ZeroSchemaPatch::FromJsonImpl(const Json::Value& /*json*/)
{
}

void ZeroSchemaPatch::Apply(ISession& session) const {
	std::stringstream createJournal;
	createJournal << "create table if not exists private_Journal";
	createJournal << " (";
	createJournal << " id integer primary key autoincrement not null,";
	createJournal << " time timestamp not null default current_timestamp,";
	createJournal << " schemaVersion bigint,";
	createJournal << " contextId text, ";
	createJournal << " patchJson text";
	createJournal << ");";

	std::stringstream createChildToParentNames;
	createChildToParentNames << "create table if not exists private_ChildToParentNames";
	createChildToParentNames << " (";
	createChildToParentNames << " child text not null unique,";
	createChildToParentNames << " parent text";
	createChildToParentNames << ");";

	std::stringstream createSchemaVersionIndex;
	createSchemaVersionIndex << "create index";
	createSchemaVersionIndex << " private_Journal_schemaVersion_idx";
	createSchemaVersionIndex << " on private_Journal (schemaVersion);";

	session.ExecSql(createJournal.str());
	session.ExecSql(createChildToParentNames.str());
	session.ExecSql(createSchemaVersionIndex.str());
}

}
}

REGISTER_PATCH(repo::sqlite::ZeroSchemaPatch)

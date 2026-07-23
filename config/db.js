const sql = require("mssql/msnodesqlv8");

const config = {
  connectionString:
    "Driver={ODBC Driver 17 for SQL Server};Server=MSI;Database=StudentManagement;Trusted_Connection=Yes;",
};

const poolPromise = sql
  .connect(config)
  .then((pool) => {
    console.log("✅ Connected to SQL Server");
    return pool;
  })
  .catch((err) => {
    console.error("❌ Database connection failed:", err);
  });

module.exports = {
  sql,
  poolPromise,
};

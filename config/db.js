const sql = require("mssql/msnodesqlv8");

const config = {
  connectionString:
    "Driver={ODBC Driver 17 for SQL Server};Server=MSI;Database=StudentManagement;Trusted_Connection=Yes;",
};

async function connectDB() {
  try {
    await sql.connect(config);
    console.log("✅ Connected to SQL Server");
  } catch (err) {
    console.error(err);
  }
}

connectDB();

module.exports = sql;

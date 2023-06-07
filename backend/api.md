# API

## Quick overview

To replace the NodeRed that was used in the previous iteration of the project we created a couple of small applications that send the data to the database. We also created a new API, named NodeRuby. It contains a simple registration form for the devices and a simple query builer using the route. 
This API is extermely basic with very little real functionality (except device registration). The main reason this API was implemented was for future iterations of this project.

The only part that might needs some explanation is how the routes are used.

```js
const express = require("express");
const router = express.Router();
const { InfluxDB, Point } = require("@influxdata/influxdb-client");

const influxDB = new InfluxDB({
  url: process.env.INFLUXDB_URL,
  token: process.env.INFLUXDB_TOKEN,
});
const queryApi = influxDB.getQueryApi(process.env.INFLUXDB_ORG);

router.get(
  "/:stationid/:fields/:start/:stop/:aggregateWindow",
  async (req, res) => {
    const stationid = req.params.stationid;
    const fields = req.params.fields.split(",");
    const start = req.params.start;
    const stop = req.params.stop;
    const aggregateWindow = req.params.aggregateWindow;

    try {
      let query = `from(bucket: "exampleBucket")
    |> range(start: ${start}, stop: ${stop})
    |> filter(fn: (r) => r["_measurement"] == "example_measurement")`;
      query += `|> filter(fn: (r) => r["_field"] == "${fields[0]}"`;
      //query += ` |> filter(fn: (r) => r["_field"] == "${field}")`;
      for (let i = 1; i < fields.length; i++) {
        query += `or r["_field"] == "${fields[i]}"`;
      }
      query += `)`;
      query += `|> filter(fn: (r) => r["source"] == "${stationid}")
      |> aggregateWindow(every: ${aggregateWindow}, fn: mean, createEmpty: false)
    |> yield(name: "mean")`;

      const rows = await queryInfluxDB(query);
      const result = { data: rows };
      res.setHeader("Content-Type", "application/json");
      res.send(JSON.stringify(result));
    } catch (error) {
      console.error(error);
      res.status(500).send("Internal server error");
    }
  }
);

function queryInfluxDB(query) {
  return new Promise((resolve, reject) => {
    const rows = [];
    queryApi.queryRows(query, {
      next(row, tableMeta) {
        rows.push(row);
      },
      error(error) {
        reject(error);
      },
      complete() {
        resolve(rows);
      },
    });
  });
}

```

The route contains some variables that, when filled out correctly, executes a query in the InfluxDB bucket and sends the result as json to the user. The route can contains multiple fields, so it is possible to get all the temperature values and all the humidity values with a sinhle query.
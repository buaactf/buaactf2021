var express = require('express');
var set = require('set-value');
var crypto = require("crypto");
var router = express.Router();
var config = require('../config.js');

function sha1(s) {
  return crypto.createHash("sha1")
      .update(s)
      .digest("hex");
}

router.get('/', function(req, res) {
  // res.render('index', {msg : "Welcome to NodeJS Lab!"});
  res.send("Welcome to NodeJS Lab!");
});

router.get("/begin", function (req, res) {
  req.session.data = {
    name : "guest"
  }
  req.session.hidden = {}
  // res.render("index", {msg : "Let's begin!"});
  res.send("Let's begin!");
});

router.post("/set", function (req, res) {
  if(req.session.data === undefined){
    res.redirect('/begin');
  } else {
  	input = req.body;
  	for(let i in input)
  	{
	    set(req.session.data, i, input[i]);
  	}
  	// res.render("index", {msg : "Set variable success!"});
  	res.send("Set variable success!");
  }
});

router.get("/flag", function (req, res) {
  const salt = "or4nge";
  
  var hidden = req.session.hidden;

  if (hidden === undefined || hidden.x === undefined || hidden.y === undefined || hidden.x.length !== hidden.y.length) {
  	  // res.render("index", {msg : "Bad input"});
  	  res.send("Bad variable");
      return;
  }

  if (hidden.x !== hidden.y && sha1(salt + hidden.x) === sha1(salt + hidden.y)) {
  	  // res.render("index", {msg : config.flag});
  	  res.send(config.flag)
      return;
  }

  // res.render("index", {msg : "Access denied"});
  res.send("Access denied");
});

module.exports = router;

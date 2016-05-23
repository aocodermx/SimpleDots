Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  // var url = 'http://192.168.1.194:8000/config/';
  var url = 'http://aocodermx.me/projects/coming/SimpleDots/config/';
  console.log('PebbleKit open configuration ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode the user's preferences
  console.log ( "Response: " + e.response );
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log ( "Config website returned: " + JSON.stringify ( configData ) );

  var data = {
    'appkWeather'    : configData['appkWeather'],
    'appkDate'       : configData['appkDate'],
    'appkDateFormat' : configData['appkDateFormat'],
    'appkHourFormat' : configData['appkHourFormat'],
    'appkBattery'    : configData['appkBattery'],
    'appkBackground' : parseInt ( configData['appkBackground'], 16 )
  };

  // Pebble.sendAppMessage(configData, function() {
  Pebble.sendAppMessage(data, function() {
    console.log('Config data sent successfully!');
  }, function(e) {
    console.log('Error sending config data!');
  });
});

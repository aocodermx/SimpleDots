var geoWatch;
var owm_apik = '9c773fb0b93229341b90eed4c3c909ad';

Pebble.addEventListener('ready', function() {
  Pebble.sendAppMessage({'appkReady': 1});
  // getUserLocation ( );
});

Pebble.addEventListener('showConfiguration', function ( ) {
  var
    //host = '192.168.1.194:8000/config/?',
    host = 'aocodermx.me/projects/coming/SimpleDots/config/?',
    url = 'http://' + host +
      'watch_platform=' + encodeURIComponent ( Pebble.getActiveWatchInfo ? Pebble.getActiveWatchInfo().platform : "aplite" ) +
      '&locationAvailable=' + encodeURIComponent ( localStorage.getItem ( 'locationAvailable' ) );
  console.log('PebbleKit open configurl: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode the user's preferences
  console.log ( "Response: " + e.response );
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log ( "Config website returned: " + JSON.stringify ( configData ) );

  var data = {
    'appkWeather'       : configData['appkWeather'],
    'appkWeatherPeriod' : configData['appkWeatherPeriod'],
    'appkWeatherUnits'  : configData['appkWeatherUnits'],

    'appkDate'       : configData['appkDate'],
    'appkDateFormat' : configData['appkDateFormat'],

    'appkHourFormat' : configData['appkHourFormat'],
    'appkBattery'    : configData['appkBattery'],
    'appkBackground' : parseInt ( configData['appkBackground'], 16 )
  };

  localStorage.setItem ( 'appkWeatherUnits', configData['appkWeatherUnits'] );
  localStorage.setItem ( 'weatherCity', configData['weatherCity'] );

  // Pebble.sendAppMessage(configData, function() {
  Pebble.sendAppMessage(data, function() {
    console.log('Config data sent successfully!');
  }, function(e) {
    console.log('Error sending config data!');
  });
});

Pebble.addEventListener('appmessage', function(e) {
  console.log('Received message ' + JSON.stringify ( e.payload ) );
  getUserLocation ( );
});


function getUserLocation ( ) {
  console.log('Setting location request')
  geoWatch = navigator.geolocation.getCurrentPosition (
    function success(pos) {
      localStorage.setItem ( 'locationAvailable', true );
      queryWeather ( 'lat=' + pos.coords.latitude + '&' + 'lon=' + pos.coords.longitude );
    },
    function error(err) {
      localStorage.setItem ( 'locationAvailable', false );
      queryWeather ( 'q=' + localStorage.getItem ( 'weatherCity' ) );
    },
    {
    enableHighAccuracy: false,
    maximumAge: 10000,
    timeout: 10000
  } );
}

function queryWeather ( params ) {
  var
    request = new XMLHttpRequest(),
    resource = 'http://api.openweathermap.org/data/2.5/weather?' + params + '&units=' + localStorage.getItem ( 'appkWeatherUnits' ) + '&appid=' + owm_apik;

  request.onload = sendWeatherBack;
  request.open ( 'GET', resource );
  request.send();

  console.log ( 'Resource to load: ' + resource );
}

function sendWeatherBack ( ) {
  if ( this.status == '200' ) {
    weather = JSON.parse ( this.responseText );
    console.log('Got response (' + this.status +'): ' + JSON.stringify ( weather ) );

    var
      dict = {},
      icon = weather['weather'][0]['icon'];

    if ( icon.indexOf( 'n' ) != -1 ) {
      icon = icon.replace ( 'n', '0' );
    }

    if ( icon.indexOf( 'd' ) != -1 ) {
      icon = icon.replace ( 'd', '1' );
    }

    dict['appkWeather_icon'] = parseInt ( icon );

    Pebble.sendAppMessage( dict, function() {
      console.log ( 'Icon sent successfully.' + JSON.stringify ( dict ) );
    }, function () {
      console.log ( 'Icon failed.' + JSON.stringify ( dict ) );
    });
  } else {
    console.log ( 'Something went wrong ' + this.status );
  }
}

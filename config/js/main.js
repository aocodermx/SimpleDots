(function() {
  processParams ( );
  loadConfiguration ( );
  setEventHandlers ( );
})();

function processParams ( ) {
  if ( getQueryParam ( "watch_platform" ) == "aplite" ) {
    $("#withBgcolor").parent().hide();
  }
  if ( getQueryParam ( "locationAvailable" ) == "true" ) {
    localStorage.setItem ( 'locationAvailable', true );
    $("#withWeatherCity").parent().parent().hide();
  } else {
    localStorage.setItem ( 'locationAvailable', false );
  }
}

function loadConfiguration ( ) {
  if ( localStorage.conf_SimpleDots ) {
    $( "#withWeather"      )[0].checked = localStorage.appkWeather === '1';
    $( "#withWeatherCity"  )[0].value   = localStorage.appkWeatherCity;
    $( "#withWeatherPeriod")[0].value   = localStorage.appkWeatherPeriod;
    $( "#withWeatherUnits" )[0].value   = localStorage.appkWeatherUnits;

    $( "#withDate"      )[0].checked = localStorage.appkDate    === '1';
    $( "#withDateFormat")[0].value   = localStorage.appkDateFormat;

    $( "#withHourFormat" )[0].value   = localStorage.appkHourFormat;
    $( "#withBatteryDots")[0].checked = localStorage.appkBattery === '1';
    $( "#withBgcolor"    )[0].value   = localStorage.appkBackground;
    onWeatherChange ( );
    onDateChange ( );
  } else {
    console.log( "No configuration saved." );
  }
}

function saveConfiguration ( config ) {
  localStorage.conf_SimpleDots= true;

  localStorage.appkWeather       = config.appkWeather;
  localStorage.appkWeatherCity   = config.appkWeatherCity;
  localStorage.appkWeatherPeriod = config.appkWeatherPeriod;
  localStorage.appkWeatherUnits  = config.appkWeatherUnits;

  localStorage.appkDate       = config.appkDate;
  localStorage.appkDateFormat = config.appkDateFormat;

  localStorage.appkHourFormat = config.appkHourFormat;
  localStorage.appkBattery    = config.appkBattery;
  localStorage.appkBackground = config.appkBackground;
}

function setEventHandlers ( ) {
  $( '#withWeather' ).on ( 'change', onWeatherChange );
  $( '#withDate'    ).on ( 'change', onDateChange );
  $( '#save_button' ).on ( 'click' , onSubmit     );
}

function onWeatherChange ( ) {
  if ( $("#withWeather")[0].checked && localStorage.getItem ( 'locationAvailable' ) != "true" ) {
    $("#withWeatherCity").parent().parent().show();
  } else {
    $("#withWeatherCity").parent().parent().hide();
  }

  if ( $("#withWeather")[0].checked ) {
    $("#optionsWeatherPeriod").show();
    $("#optionsWeatherUnits").show();
  } else {
    $("#optionsWeatherPeriod").hide();
    $("#optionsWeatherUnits").hide();
  }
}

function onDateChange ( ) {
  if ( $("#withDate")[0].checked ) {
    $("#optionsDate").show ( );
  } else {
    $("#optionsDate").hide ( );
  }
}

function onSubmit ( ) {
  var
    appkWeather       = $( "#withWeather"     )[0].checked ? 1 : 0,
    appkWeatherCity   = $( "#withWeatherCity" ).val(),
    appkWeatherUnits  = $( "#withWeatherUnits").val(),
    appkWeatherPeriod = parseInt ( $( "#withWeatherPeriod").val() ),

    appkDate        = $( "#withDate" )[0].checked ? 1 : 0,
    appkDateFormat  = parseInt ( $( "#withDateFormat" ).val( ) ),

    appkHourFormat  = parseInt ( $( "#withHourFormat" ).val( ) ),
    appkBattery     = $( "#withBatteryDots")[0].checked ? 1 : 0,
    appkBackground  = $( "#withBgcolor"    ).val(),

  config = {
    'appkWeather'      : appkWeather,
    'appkWeatherCity'      : appkWeatherCity,
    'appkWeatherPeriod': appkWeatherPeriod,
    'appkWeatherUnits'     : appkWeatherUnits,

    'appkDate'         : appkDate,
    'appkDateFormat'   : appkDateFormat,

    'appkHourFormat'   : appkHourFormat,
    'appkBattery'      : appkBattery,
    'appkBackground'   : appkBackground,
  };

  saveConfiguration ( config );

  var return_to = getQueryParam('return_to', 'pebblejs://close#');
  document.location = return_to + encodeURIComponent ( JSON.stringify ( config ) );
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}

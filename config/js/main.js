
(function() {
  loadConfiguration();
  setEventHandlers();
})();

function loadConfiguration ( ) {
  if ( localStorage.conf_SimpleDots ) {
    $( "#withDateFormat"  )[0].value   = localStorage.appkDateFormat;
    $( "#withHourFormat"  )[0].value   = localStorage.appkHourFormat;
    $( "#withBgcolor"     )[0].value   = localStorage.appkBackground;
    $( "#withWeather"     )[0].checked = localStorage.appkWeather    === '1';
    $( "#withDate"        )[0].checked = localStorage.appkDate       === '1';
    $( "#withBatteryDots" )[0].checked = localStorage.appkBattery    === '1';
    onDateChange ( );
  } else {
    console.log( "No configuration saved." );
  }
}

function saveConfiguration ( config ) {
  localStorage.conf_SimpleDots= true;
  localStorage.appkWeather    = config.appkWeather;
  localStorage.appkDate       = config.appkDate;
  localStorage.appkDateFormat = config.appkDateFormat;
  localStorage.appkHourFormat = config.appkHourFormat;
  localStorage.appkBattery    = config.appkBattery;
  localStorage.appkBackground = config.appkBackground;
}

function setEventHandlers ( ) {
  $( '#withDate'    ).on( 'change', onDateChange );
  $( '#save_button' ).on( 'click' , onSubmit     );
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
    appkBackground  = $( "#withBgcolor"     ).val(),
    appkDateFormat  = parseInt ( $( "#withDateFormat" ).val( ) ),
    appkHourFormat  = parseInt ( $( "#withHourFormat" ).val( ) ),
    appkWeather     = $( "#withWeather"     )[0].checked ? 1 : 0,
    appkDate        = $( "#withDate"        )[0].checked ? 1 : 0,
    appkBattery     = $( "#withBatteryDots" )[0].checked ? 1 : 0,

  config = {
    'appkWeather'    : appkWeather,
    'appkDate'       : appkDate,
    'appkDateFormat' : appkDateFormat,
    'appkHourFormat' : appkHourFormat,
    'appkBattery'    : appkBattery,
    'appkBackground' : appkBackground,
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

// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "on"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 37.4236, lng: -121.9}, // SanJose, California
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);
    
});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    //Getting lat and lng from place object
     var loc ={};
     loc["lat"]= place.latitude;
     loc["lng"]= place.longitude;
    
    var iconBase = 'https://maps.google.com/mapfiles/kml/';
       
    /* Help from https://github.com/davidventuri/harvard-cs50/blob/master/pset8/public/index.html    */
    var marker = new MarkerWithLabel({
        position: loc,
        map: map,
        icon: iconBase + 'pal2/icon31.png',
        labelContent: place.place_name + ", " + place.admin_code1,
        labelAnchor: new google.maps.Point(50, 0),
        labelClass: "labels",
        labelStyle: {opacity: 0.75}
        });
        
    markers.push(marker);
    
    
    marker.addListener('click', function() {
       setcontent(place,marker);
           
        }); 
}


function setcontent(place, marker)
{
     
    var parameters = {
        geo: place.postal_code
        };
        
    $.getJSON(Flask.url_for("articles"), parameters)
    .done(function(data, textStatus, jqXHR) {
   
       makehtml(marker, data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        console.log(errorThrown.toString());
    });
}

function makehtml(marker,data)
{
   if (data.length == 0)
    {
        showInfo(marker, "no news for this area");
    }
    else {

        var mydiv= "<ul>";
        
        for(var i =0; i< data.length; i++){
         a = '<li><a href="' + data[i].link+ '">' + data[i].title +'</a></li>';
         mydiv += a;
        }
          
    }

    mydiv += '</ul>';
    showInfo(marker, mydiv);
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        div+=content
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}




/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // if info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) { return null; },
        limit: 10,
        source: search,
        templates: {
            suggestion: Handlebars.compile(
                "<div>" +
                "{{place_name}}, {{admin_name1}}, {{postal_code}}" +
                "</div>"
            )
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // set map's center
        map.setCenter({lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude)});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

//helps remove markers
function setMapOnAll(map) {
        for (var i = 0; i < markers.length; i++) {
          markers[i].setMap(map);
        }
      }

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    setMapOnAll(null);
    markers = [];
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("search"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());

        // call typeahead's callback with no results
        asyncResults([]);
    });
}




/**
 * Updates UI's markers.
 */
function update() 
{
     
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    
    $.getJSON(Flask.url_for("update"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
       // remove old markers from map
       removeMarkers();
       
       console.log(data.length);
       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
        
           addMarker(data[i]);
           
       }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}

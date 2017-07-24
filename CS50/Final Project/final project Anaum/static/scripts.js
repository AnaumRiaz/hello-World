
// Google Map
var map;
var markerCluster;
      
// markers for map
var markers = [];
var check = 1;   //normal view

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {


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


      // Try HTML5 geolocation.
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(function(position) {
        var pos = {
          lat: position.coords.latitude,
          lng: position.coords.longitude
        };
        var pls = {
            "latitude": position.coords.latitude,
            "longitude": position.coords.longitude,
            "place_name": "Your Location",
            "admin_code1": ""
        };

        info.setPosition(pos);
        map.setCenter(pos);
        addMarker(pls);
    
        }, function() {
        handleLocationError(true, info, map.getCenter());
        });
        } 
    else {
      // Browser doesn't support Geolocation
      handleLocationError(false, info, map.getCenter());
     }
      

    function handleLocationError(browserHasGeolocation, info, pos) {
        info.setPosition(pos);
        info.setContent(browserHasGeolocation ?
                              'Error: The Geolocation service failed.' :
                              'Error: Your browser doesn\'t support geolocation.');
     var pos = {
          lat: 37,
          lng: -120
        };
        var pls = {
            "latitude": 37,
            "longitude": -120,
            "place_name": "",
            "admin_code1": "  "
        };

        info.setPosition(pos);
        map.setCenter(pos);
        addMarker(pls);
    }



    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 10,
        zoomControl: true
    };


    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);
  
    var centerControlDiv = document.createElement('div');
    var centerControl = new CenterControl(centerControlDiv, map);

    centerControlDiv.index = 1;
    map.controls[google.maps.ControlPosition.TOP_LEFT].push(centerControlDiv);
    
    var centerControlDiv2 = document.createElement('div');
    var centerControl2 = new CenterControl1(centerControlDiv2, map);

    centerControlDiv2.index = 1;
    map.controls[google.maps.ControlPosition.TOP_RIGHT].push(centerControlDiv2);


});



//introducing button1

function CenterControl(controlDiv, map) {

  // Set CSS for the control border.
  var controlUI = document.createElement('div');
  controlUI.style.backgroundColor = '#fff';
  controlUI.style.border = '2px solid #fff';
  controlUI.style.borderRadius = '3px';
  controlUI.style.boxShadow = '0 2px 6px rgba(0,0,0,.3)';
  controlUI.style.cursor = 'pointer';
  controlUI.style.marginBottom = '22px';
  controlUI.style.textAlign = 'center';
  controlUI.title = 'Click to see attractions';
  controlDiv.appendChild(controlUI);

  // Set CSS for the control interior.
  var controlText = document.createElement('div');
  controlText.style.color = 'rgb(25,25,25)';
  controlText.style.fontFamily = 'Roboto,Arial,sans-serif';
  controlText.style.fontSize = '16px';
  controlText.style.lineHeight = '38px';
  controlText.style.paddingLeft = '5px';
  controlText.style.paddingRight = '5px';
  controlText.innerHTML = 'Attractions';
  controlUI.appendChild(controlText);
  
  // Setup the click event listeners: 
  controlUI.addEventListener('click', function() {

  if (check == 0)
  {
      check = 1;
      controlText.innerHTML = 'Attractions';}
  else 
  {
      check = 0;
      controlText.innerHTML = 'Normal';
  }
  update();
  });
}

//introducing button 2
function CenterControl1(controlDiv, map) {

  // Set CSS for the control border.
  var controlUI = document.createElement('div');
  controlUI.style.backgroundColor = '#fff';
  controlUI.style.border = '2px solid #fff';
  controlUI.style.borderRadius = '3px';
  controlUI.style.boxShadow = '0 2px 6px rgba(0,0,0,.3)';
  controlUI.style.cursor = 'pointer';
  controlUI.style.marginBottom = '22px';
  controlUI.style.textAlign = 'center';
  controlUI.title = 'Send the list';
  controlDiv.appendChild(controlUI);

  // Set CSS for the control interior.
  var controlText = document.createElement('div');
  controlText.style.color = 'rgb(25,25,25)';
  controlText.style.fontFamily = 'Roboto,Arial,sans-serif';
  controlText.style.fontSize = '16px';
  controlText.style.lineHeight = '38px';
  controlText.style.paddingLeft = '5px';
  controlText.style.paddingRight = '5px';
  controlText.innerHTML = 'Email list';
  controlUI.appendChild(controlText);
  
  // Setup the click event listeners: 
  controlUI.addEventListener('click', function() {
  semail();
  });
}

function semail()
{
    var e_add = prompt("Please enter email address")
    var re = /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
    if (re.test(e_add))
    {
    var parameters = {
        address: e_add
        };
    //window.open('mailto:test@example.com');   
    $.getJSON(Flask.url_for("sendemail"), parameters)
    .done(function(data, textStatus, jqXHR) {
       alert("email sent");
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
         alert("couldn't send email");
        console.log(errorThrown.toString());
    });
    }
    else
    {
        alert("email address not valid");
    }
    
}

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
    var iconf = 'pal2/icon31.png'; 
    
     
    if (place.admin_code1 == "")
    {
        iconf = 'pal5/icon6.png';  
    }
    else if (!place.admin_name1)
    {
        iconf = 'shapes/placemark_circle.png';
    }
   
    /* Help from https://github.com/davidventuri/harvard-cs50/blob/master/pset8/public/index.html    */
    var marker = new MarkerWithLabel({
        position: loc,
        map: map,
        icon: iconBase + iconf,
        labelContent: place.place_name + ", " + place.admin_code1,
        labelAnchor: new google.maps.Point(50, 0),
        labelClass: "labels",
        labelStyle: {opacity: 0.75}
        });
    
    if (place.admin_code1 == "")
    {
        marker.bindTo(loc, 'map');
    }
    
    markers.push(marker);
  
 
    marker.addListener('click', function() {
       setcontent(place,marker);
           
        }); 
        
}


 
    
function setcontent(place, marker)
{
    if (check == 1)
    {
    var parameters = {
        geo: place.postal_code,
        pname: place.place_name
        };
        
    $.getJSON(Flask.url_for("articles"), parameters)
    .done(function(data, textStatus, jqXHR) {
       
       makehtml(marker, data,place);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        console.log(errorThrown.toString());
    });
    }
    else if (check == 0)  //attractions view
    {
        parameters = {
        pname: place.place_name
        };
        
    $.getJSON(Flask.url_for("att"), parameters)
    .done(function(data, textStatus, jqXHR) {
       
       makehtml(marker, data, place);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        console.log(errorThrown.toString());
    });
    
    }
    
}

function makehtml(marker,data, place)
{
    
    if (data.length == 0) {
        showInfo(marker, "no info");
    }
    
    else {

        if (check == 1) {
            var mydiv = "<ul>";
            dl = data.length;

            for (var i = 0; i < (dl - 2); i++) {
                a = '<li><a href="' + data[i].link + '">' + data[i].title + '</a></li>';
                mydiv += a;
            }
            mydiv += '</ul>';
            var temp = data[dl - 2].Temperature;
            var time = data[dl - 1].Time;
       
        }
        else {
            
            if (data[0].mode == "saved")
            {
                status = 'Delete';
            }
            else
            {
                status = 'Save it';
            }
            
             link = '<a title="Click to save it to your list" href="#" onclick="saveunsave(\''+place.place_name+'\');return false;">' + status + '</a>';

             temp = '<a href="' + data[0].link1 + '">' + "Wikipedia" + '</a>';
             linkitem= '<div id = "saveornot">' + link +' </div>';
             time = '<a href="' + data[0].link2 + '">' + "Google images" + '</a><br/><li>'+ linkitem ;
             
             mydiv = "<img src='/static/pics/"+ data[0].fn +"'/>";
            
             
             
             
        }
    }
    
    
    showInfo(marker, mydiv, temp, time);
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content, temp, time)
{
    // start div
    if (check == 0)
    {
         var div = "<div id='info' style='float:left; width:55%;'>";
    }
    else 
    {
          var div = "<div id='info' style='float:left; width:70%;'>";
    }
    
   
    
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        div+=content;
    }

    // end div
   
    div += "</div>";
    
    if (check == 1)   //normal view
    {
        div += "<div style='float:right; width:20%;'>";
        div += "<br/><br/><br/><b>TEMPERATURE</b><br/><br/><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
        div +=  temp;
        div += "<br/><br/><br/><br/><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TIME</b><br/><br/>&nbsp;&nbsp;&nbsp;<b>";
        div += (time + "</b>");
    }
    else{
        div += "<div style='float:right; width:45%;'>";
        div += "</br><ul><li>" + temp + "</li><li>" + time + "</li>";
    
    }
    
    div += "</div>";
      
    // set info window's content
    info.setContent(div);
    

    // open info window (if not already open)
    info.open(map, marker);
} 


function saveunsave(place)
{
     var parameters = {
        place: place
    };
    $.getJSON(Flask.url_for("saveodelete"), parameters)
    .done(function(data, textStatus, jqXHR) {
     document.getElementById("saveornot").innerHTML = data[0]['newstatus']
        
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());

    });
    
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
   // update();

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
    mode = check;
   
    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng(),
        mode: check  //0 for attractions, 1 for normal
    };
     
     
    if (check == 0 )
    {
        
        if (markerCluster) {
          
          markerCluster.clearMarkers();
          markerCluster.setMap(null);
          }
      
    }
    
    $.getJSON(Flask.url_for("update"), parameters)
    .done(function(data, textStatus, jqXHR) {
       // remove old markers from map
       removeMarkers();
       
       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
            addMarker(data[i]);
       }

   if (check == 0)
    {
        markerCluster  = new MarkerClusterer(map, markers, { imagePath: 'static/clusterimgs/'});
    }
    
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}
Working Title: RCCar Maps Application
=====================================

A Repository that contains the android application for the CMPE243 project.

09/24/2015: "First Commit"
- Created Repository. Made first commit. Pushed everything that was done online.
- Implemented a basic Maps fragment with a buttons for a source, destination and send info.
- Using a basic marker that comes as part of the maps API.

things to consider-
- Routes API to get directions between two points.
- using the clickListener method that comes with the Maps API.

"Added README.md"
- Created a README as a means to set an agendas and detail changes and implementations made to the app.		

"Used OnMapClick"
- Used the OnMapClick event that is part of the maps API as opposed to a ClickListener event that's connected to a button
- Marker now appears at the center of the screen as opposed to a preset location on screen. Solves the problem of having to move around to find a pointer
- Added a Route Button.

"Added a JSONParser Class"
- Added a JSON Parser to parse a string.

"Start plotting routes"
- Map plots routes from point A to point B.
- Doesn't work for roads within University campus.


"Routes within University"
- Maps now routes within the campus. It was all about specifying the mode as walking.
- Logging of intermediate co-ordinates done. Is it less than 8 points? because Google Maps free limits intermediate waypoints to 8.
- removed duplicate markers.
- added a URL for Roads API request. Roads API has a breadcrumbs feature which can hopefully be used to implement intermediate markers.

"Intermediate points"
- Added a Map Icon
- Added a Method that checks if phone is connected to the internet
- puts intermediate points on the map. Can be controlled by means of a simple filter. 

things to consider-
- Use elevations api?
- Roads API and Breadcrumbs?
- make the filter more robust
- add a app toolbar for better UX
- integrate with anush's bluetooth part of the map?


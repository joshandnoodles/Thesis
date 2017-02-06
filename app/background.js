chrome.app.runtime.onLaunched.addListener( function() {
  chrome.app.window.create( 'index.html', {
    'innerBounds': {
      'width': 731,
      'height': 823
    },
  }, function(win) {
    win.outerBounds.setPosition(
      0,    // left
      0     // top
    )
  } );
} )
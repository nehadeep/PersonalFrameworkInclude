var gulp = require('gulp');
var concat = require('gulp-concat');
var uglify = require('gulp-uglify');
var handleErrors = require('../util/handleErrors');
var argv = require('yargs').argv;
var chmod = require('gulp-chmod');
var config = require('../config').bundle;
var drop = (argv.path) ? argv.path + '/' : './';

var files = [
    './gui/javascript/global.js',
    './gui/javascript/glwindow.js',
    './gui/javascript/JQuery/1.7.1/jquery.min.js',
    './gui/javascript/JQuery/JSON/json3.min.js',
    './gui/javascript/JQuery/UI/jquery.ui.base.js',
    './gui/javascript/JQuery/tools/jquery.tools.1.2.2.min.js',
    './gui/javascript/JQuery/UI/blockUI/2.09/jquery.blockUI-2.09.js',
    './gui/javascript/ShipStatus.js',
    './gui/javascript/Placeholders.min.js',
    './gui/javascript/JQuery/tmpl/1.0.0/jquery.tmpl.min.js',
    './gui/javascript/controls/jquery.socialshare.js',
    './gui/javascript/controls/jquery.multibutton.js',
    './gui/javascript/controls/jquery.multilink.js',
    './gui/javascript/controls/jQueryUtilityPlugins.js',
    './gui/javascript/controls/ScrollableRecoPanel.js',
    './gui/javascript/controls/CartConfirmationNugget.js',
    './gui/javascript/controls/Popover.js',
    './gui/javascript/controls/AddToCartNugget.js',
    './gui/javascript/controls/SaveAsFavorite.js',
    './gui/javascript/controls/CdwRecommendations.js'
]

//Removed     './gui/javascript/controls/jquery.ui.tabs.custom.js' What is this for (tabs included in jquery.tools?
//Removed     './gui/javascript/controls/CompareCheckboxes.js' 12/16/15 -CT
//Removed     './gui/javascript/controls/inkandtonerfinder.js', 2016-04-08 -BP

gulp.task('bundle', function () {
    return gulp.src(files)
      .pipe(chmod(755))
      .pipe(concat('bundle.js'))
      .on('error', handleErrors)
      .pipe(uglify())
      .pipe(gulp.dest(config.dest));
});
var gulp      = require('gulp');
var config = require('../config').css;
var handleErrors = require('../util/handleErrors');
var minifyCSS = require('gulp-clean-css');
var chmod = require('gulp-chmod');

gulp.task('minifyCss', ['sass'], function () {
    console.log('Running CSS minify on ' + config.src + ' to ' + config.dest);
    return gulp.src(config.src)
    .pipe(chmod(755))
    .pipe(minifyCSS({ compatibility: 'ie8' }))
    .on('error', handleErrors)
    .pipe(gulp.dest(config.dest))
})

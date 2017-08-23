var gulp    = require('gulp');
var config  = require('../config').js;
var uglify = require('gulp-uglify');
var chmod = require('gulp-chmod');

gulp.task('uglifyJs', ['browserify'], function () {
  console.log('Running JS minify on ' + config.src + ' to ' + config.dest);
  return gulp.src(config.src)
    .pipe(chmod(755))
    .pipe(uglify())
    .pipe(gulp.dest(config.dest))
});

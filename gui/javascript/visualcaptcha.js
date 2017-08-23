var VisualCaptchaService = function () {
    var getTokenUri = '/api/VisualCaptcha/GenerateVisualCaptcha/',
        questionText = 'Click or touch the ',
        token = '',
        $container,
        methods;

    var getJsonFromServer = function (url) {
        return $.ajax({
            url: url,
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            method: 'GET'
        });
    };

    var getToken = function () {
        var queryString = '?isRetina=false&token=' + token;
        return getJsonFromServer(getTokenUri + queryString);
    };

    var buildImage = function (src) {
        var $img = $('<img/>', {
            'src': 'data:image/png;base64,' + src,
            'class': 'js-image-option',
            'data-index': 0
        });
        return $img;
    };

    var buildImageContainer = function() {
        return $("<div />", { 'class': 'image-container' });
    };

    var buildImageWrapper = function() {
        return $("<div />", { 'class': 'image-wrapper', 'onclick': 'CdwTagMan.createElementPageTag(\'Create Account\',\'Account Creation Captcha 3\')'});
    };

    var buildQuestion = function (src) {
        var $q = $('<p />', {
            html: questionText
        });
        var $s = $('<span />',
        {
            html: src
        });
        $q.append($s);
        return $q;
    };

    var buildRefresh = function () {
        var $refresh = $("<div />", { 'class': 'visualcaptcha-refresh-button js-refresh' }).append();
        $refresh.append($("<a />", { 'href': '#', 'class': 'refresh', 'title': 'Refresh' }));
        return $refresh;
    };

    var buildMessageWrapper = function() {
        return $("<div />", { 'class': 'message-wrapper' });
    };

    var buildSuccessMessage = function () {
        var $wrapper = buildMessageWrapper();
        var $m = $('<p />', {
            html: ("Selection Captured")
        });
        $wrapper.append($m);
        return $wrapper;
    };

    var convertErrorMessage = function () {
        var errorKey = "";
        var cancelWrapper = { cancel: false };

        if (typeof methods.determineErrorKey == "function")
            errorKey = methods.determineErrorKey(cancelWrapper);

        if (cancelWrapper.cancel === true)
            return;
        
        var newMessage;
        switch (errorKey.toLowerCase()) {
            case 'failedimage':
                newMessage = 'The image selected is incorrect. Please try again.';
                break;
            case 'nocaptcha':
                newMessage = 'We didn’t recognize your selected image. Please try again.';
                break;
            case "tokentimeout":
                newMessage = 'A system time out occurred. Please try again.';
                break;
            default:
                newMessage = 'A system error occurred. Please try again.';
                break;
        }

        if (typeof methods.showError == "function")
            methods.showError(newMessage);
        
        CdwTagMan.createElementPageTag(document.title, 'Visual Captcha - Error');
    };

    var handleRefresh = function () {
        if (typeof methods.onRefresh == 'function')
            methods.onRefresh();
    };

    var refresh = function () {
        var $vcapContainer = $("#VisualCaptchaContainer", $container);
        CdwTagMan.createElementPageTag(document.title, 'Visual Captcha - Refresh');
        $vcapContainer.css('min-height', $vcapContainer.height());
        $vcapContainer.addClass("out");
        $vcapContainer.off("click", ".js-refresh");
        window.setTimeout(function () {
            $vcapContainer.html("");

            $("#hidVisualCaptchaSelectedXAxis", $container).val('');
            $("#hidVisualCaptchaSelectedYAxis", $container).val('');
            $("#hidVisualCaptchaToken", $container).val('');

            getToken(token).then(build).then(handleRefresh);
        }, 300);
        
        return false;
    };

    var build = function (data) {
        var response = data;
        token = response.Token;
        var question = response.Question;
        var challengeImage = response.ChallengeImage;
        var $vcapContainer = $("#VisualCaptchaContainer", $container).addClass('clearfix');
        var $imageContainer = buildImageContainer();
        var $imageWrapper = buildImageWrapper();

        $("#hidVisualCaptchaToken", $container).val(token);
        $("#hidVisualCaptchaSelectedXAxis", $container).val('');
        $("#hidVisualCaptchaSelectedYAxis", $container).val('');

        $vcapContainer.removeClass('error');

        $vcapContainer.append(buildQuestion(question));

        $imageWrapper.append(buildImage(challengeImage));

        $imageContainer.append($imageWrapper);

        $vcapContainer.append($imageContainer);
        $vcapContainer.append(buildRefresh());

        window.setTimeout(function () {
            $vcapContainer.removeClass('first').removeClass('out');
        }, 300);


        $imageContainer.on("click", ".js-image-option", function (e) {
            var offset = $('.js-image-option', $container).offset();
            var selectedXAxis = Math.round(e.pageX) - Math.round(offset.left);
            var selectedYAxis = Math.round(e.pageY) - Math.round(offset.top);

            $("#hidVisualCaptchaSelectedXAxis", $container).val(selectedXAxis);
            $("#hidVisualCaptchaSelectedYAxis", $container).val(selectedYAxis);
            $("#valVisualCaptchaInvalid", $container).hide();
            $("#VisualCaptchaContainer > p", $container).hide();
            
            $imageContainer.html(buildSuccessMessage());
            
            if (typeof methods.onClick == 'function')
                methods.onClick();
        });

        $vcapContainer.on("click", ".js-refresh", refresh);
        
        if (typeof methods.onBuild == 'function')
            methods.onBuild();
    };
    var handleOnInit = function () {
        if (typeof methods.onInit == 'function')
            methods.onInit();
    };

    var init = function ($scope, handlers) {
        $container = $scope;
        methods = handlers || {};

        getToken().then(build).then(convertErrorMessage).then(handleOnInit);
    };

    return {
        init: init
    }
};
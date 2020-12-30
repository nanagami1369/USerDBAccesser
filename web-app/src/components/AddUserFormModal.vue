<template>
  <modal name="add-user-form-modal" id="add-user-form-modal" :height="350">
    <ValidationObserver :immediate="true" v-slot="{ invalid }">
      <form id="add-user-form">
        <h4>アカウントの追加</h4>
        <label for="add-user-input-name">名前 </label>
        <ValidationProvider :immediate="true" rules="required" v-slot="{ errors }">
          <input type="text" name="名前" id="add-user-input-name" v-model="addFormUser.name" />
          <span>{{ errors[0] }}</span>
        </ValidationProvider>
        <label for="add-user-input-password">パスワード(8文字以上)</label>
        <ValidationProvider :immediate="true" rules="required|min:8" v-slot="{ errors }">
          <input
            type="password"
            name="パスワード"
            id="add-user-input-password"
            v-model="addFormUser.pass"
          />
          <span>{{ errors[0] }}</span>
        </ValidationProvider>
        <label>状態</label>
        <div id="avail-radio-button-aria">
          <label for="avail-radio-true">
            <input
              type="radio"
              name="avail"
              value="true"
              id="avail-radio-true"
              v-model="addFormUser.avail"
            />有効
          </label>
          <label for="avail-radio-false">
            <input
              type="radio"
              name="avail"
              value="false"
              id="avail-radio-false"
              v-model="addFormUser.avail"
            />無効
          </label>
        </div>
        <label>権限</label>
        <div id="level-radio-button-aria">
          <label for="level-radio-admin">
            <input
              type="radio"
              name="level"
              value="ADMIN"
              id="level-radio-admin"
              v-model="addFormUser.level"
            />管理者
          </label>
          <label for="level-radio-prem">
            <input
              type="radio"
              name="level"
              value="PREM"
              id="level-radio-prem"
              v-model="addFormUser.level"
            />プレミア
          </label>
          <label for="level-radio-gen">
            <input
              type="radio"
              name="level"
              value="GEN"
              id="level-radio-gen"
              v-model="addFormUser.level"
            />一般
          </label>
          <label for="level-radio-try">
            <input
              type="radio"
              name="level"
              value="TRY"
              id="level-radio-try"
              v-model="addFormUser.level"
            />お試し
          </label>
        </div>
        <input
          id="send-form-add-user-button"
          type="button"
          @click="submit"
          value="アカウントを追加する"
          :disabled="invalid"
        />
        <p v-show="invalid" class="error-message">入力ミスがあります。</p>
      </form>
    </ValidationObserver>
  </modal>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator'
import { ValidationProvider, ValidationObserver, extend } from 'vee-validate'
import { required, min } from 'vee-validate/dist/rules'
import { User } from '@/model/User'

extend('positive', value => {
  return value >= 0
})
extend('min', {
  ...min,
  message: '{_field_}が短すぎます{length}文字以上にしてください'
})
extend('required', {
  ...required,
  message: '{_field_}は必須です'
})
@Component({
  components: { ValidationProvider, ValidationObserver }
})
export default class AddUserFormModal extends Vue {
  public addFormUser: User = {
    id: -1,
    name: '',
    pass: '',
    avail: true,
    level: 'ADMIN'
  }

  public submit() {
    this.$emit('submit', this.addFormUser)
    this.addFormUser = {
      id: -1,
      name: '',
      pass: '',
      avail: true,
      level: 'ADMIN'
    }
    this.$modal.hide('add-user-form-modal')
  }
}
</script>
<style scoped>
#add-user-form {
  display: flex;
  flex-direction: column;
  margin: 10px;
}

.error-message {
  text-decoration: underline;
}
.error-message::before {
  content: '※ ';
}
</style>
